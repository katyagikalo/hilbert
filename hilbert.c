#include <stdbool.h>
#include <xmmintrin.h>

#include <pthread.h>
#include "main.h"

void hilbert_V0(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V1(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V2(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V3(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V4(unsigned degree, coord_t* x, coord_t* y, bool use_simd);

void add_segments(unsigned degree, coord_t* x, coord_t* y);
void add_segments_simd(unsigned segment_degree, coord_t* x, coord_t* y);
void * add_segments_simd_multithreaded(void * args);
void * add_segments_multithreaded(void * args);

typedef struct{
    unsigned long long segment_length;
    unsigned segment_coord;
    unsigned start;
    unsigned end;
    coord_t *x;
    coord_t *y;
}pthread_args;

void add_segments(unsigned segment_degree, coord_t* x, coord_t* y){
    unsigned long long segment_length = (unsigned long long)1 << (2 * (segment_degree));
    unsigned segment_coord = (1 << segment_degree);

    coord_t *vx = x, *vy = y;

    //2*segment_length
    unsigned long long d_segment_length = segment_length + segment_length;
    //3*segment_length
    unsigned long long t_segment_length = d_segment_length + segment_length;

    for(unsigned long long i = 0; i < segment_length; ++i) {
        //left upper segment
        vx[segment_length].val = vx[0].val;
        vy[segment_length].val = vy[0].val + segment_coord;

        //right upper segment
        vx[d_segment_length].val = vx[0].val + segment_coord;
        vy[d_segment_length].val = vy[0].val + segment_coord;

        //left lower segment
        unsigned temp = vx[0].val;
        vx[0].val = vy[0].val;
        vy[0].val = temp;

        //right lower segment
        vx[t_segment_length].val = 2*segment_coord - 1 - vx[0].val;
        vy[t_segment_length].val = segment_coord - 1 - vy[0].val;

        vx++;
        vy++;
    }
}


void hilbert_V0(unsigned degree, coord_t* x, coord_t* y) {
    //curve for degree = 1
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    for(unsigned i = 1; i < degree; ++i){
        add_segments(i, x, y);
    }
}

void hilbert_V1(unsigned degree, coord_t* x, coord_t* y) {
    v_assembly(degree, x, y);
}

void add_segments_simd(unsigned segment_degree, coord_t* x, coord_t* y){
    unsigned long long segment_length = (unsigned long long)1 << (2 * (segment_degree));
    unsigned segment_coord = (1 << segment_degree);

    coord_t *vx = x, *vy = y;

    //2*segment_length
    unsigned long long d_segment_length = 2 * segment_length;
    //3*segment_length
    unsigned long long t_segment_length = d_segment_length + segment_length;

    __m128i arr_x, arr_y;

    __m128i sc = _mm_set1_epi32(segment_coord);
    __m128i d_sc = _mm_add_epi32(sc, sc);
    __m128i one = _mm_set1_epi32(1);

    for(unsigned long long i = 0; i < segment_length; i+=4) {

        arr_x = _mm_loadu_si128((__m128i const*)(vx));
        arr_y = _mm_loadu_si128((__m128i const*)(vy));

        //left upper segment
        _mm_storeu_si128((__m128i*)(vx + segment_length), arr_x);
        _mm_storeu_si128((__m128i*)(vy + segment_length), _mm_add_epi32(arr_y, sc));

        //right upper segment
        _mm_storeu_si128((__m128i*)(vx + d_segment_length), _mm_add_epi32(arr_x, sc));
        _mm_storeu_si128((__m128i*)(vy + d_segment_length), _mm_add_epi32(arr_y, sc));

        //left lower segment
        _mm_storeu_si128((__m128i*)(vx), arr_y);
        _mm_storeu_si128((__m128i*)(vy), arr_x);

        //right lower segment
        _mm_storeu_si128((__m128i*)(vx + t_segment_length), _mm_sub_epi32(_mm_sub_epi32(d_sc, one), arr_y));
        _mm_storeu_si128((__m128i*)(vy + t_segment_length), _mm_sub_epi32(_mm_sub_epi32(sc, one), arr_x));

        vx+=4;
        vy+=4;
    }
}

void hilbert_V2(unsigned degree, coord_t* x, coord_t* y){
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    for(unsigned i = 1; i < degree; ++i){
        add_segments_simd(i, x, y);
    }
}

void hilbert_V3(unsigned degree, coord_t* x, coord_t* y){
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    if (degree == 1){
        return;
    }

    else {
        hilbert_V3(degree-1, x, y);
        add_segments_simd(degree-1, x, y);
    }
}


void * add_segments_simd_multithreaded(void * args){
    pthread_args* temp_args = (pthread_args*) args;

    coord_t *vx = temp_args->x + temp_args->start, *vy = temp_args->y + temp_args->start;

    //2*segment_length
    unsigned long long d_segment_length = 2 * temp_args->segment_length;
    //3*segment_length
    unsigned long long t_segment_length = d_segment_length + temp_args->segment_length;

    __m128i arr_x, arr_y;

    __m128i sc = _mm_set1_epi32(temp_args->segment_coord);
    __m128i d_sc = _mm_add_epi32(sc, sc);
    __m128i one = _mm_set1_epi32(1);
    
    for(unsigned long long i = temp_args->start; i < temp_args->end; i+=4) {

        arr_x = _mm_loadu_si128((__m128i const*)(vx));
        arr_y = _mm_loadu_si128((__m128i const*)(vy));

        //left upper segment
        _mm_storeu_si128((__m128i*)(vx + temp_args->segment_length), arr_x);
        _mm_storeu_si128((__m128i*)(vy + temp_args->segment_length), _mm_add_epi32(arr_y, sc));

        //right upper segment
        _mm_storeu_si128((__m128i*)(vx + d_segment_length), _mm_add_epi32(arr_x, sc));
        _mm_storeu_si128((__m128i*)(vy + d_segment_length), _mm_add_epi32(arr_y, sc));

        //left lower segment
        _mm_storeu_si128((__m128i*)(vx), arr_y);
        _mm_storeu_si128((__m128i*)(vy), arr_x);

        //right lower segment
        _mm_storeu_si128((__m128i*)(vx + t_segment_length), _mm_sub_epi32(_mm_sub_epi32(d_sc, one), arr_y));
        _mm_storeu_si128((__m128i*)(vy + t_segment_length), _mm_sub_epi32(_mm_sub_epi32(sc, one), arr_x));
        
        vx+=4;
        vy+=4;
    }
    return NULL;
}

void * add_segments_multithreaded(void * args){
    pthread_args* temp_args = (pthread_args*) args;

    coord_t *vx = temp_args->x, *vy = temp_args->y;

    //2*segment_length
    unsigned long long d_segment_length = 2 * temp_args->segment_length;
    //3*segment_length
    unsigned long long t_segment_length = d_segment_length + temp_args->segment_length;
    
    vx+=temp_args->start;
    vy+=temp_args->start;

    for(unsigned long long i = temp_args->start; i < temp_args->end; ++i) {
        //left upper segment
        vx[temp_args->segment_length].val = vx[0].val;
        vy[temp_args->segment_length].val = vy[0].val + temp_args->segment_coord;

        //right upper segment
        vx[d_segment_length].val = vx[0].val + temp_args->segment_coord;
        vy[d_segment_length].val = vy[0].val + temp_args->segment_coord;

        //left lower segment
        unsigned temp = vx[0].val;
        vx[0].val = vy[0].val;
        vy[0].val = temp;

        //right lower segment
        vx[t_segment_length].val = 2 * temp_args->segment_coord - 1 - vx[0].val;
        vy[t_segment_length].val = temp_args->segment_coord - 1 - vy[0].val;
        
        vx++;
        vy++;
    }
    
    return NULL;
}

void hilbert_V4(unsigned degree, coord_t* x, coord_t* y, bool use_simd){
    
    unsigned const THREADS = 8;
    unsigned const START_MULTITHREADING = 4;
    //curve for degree = 1
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;
    
    if(degree == 1){
        return;
    }
    
    //calc without Multitreading
    for (unsigned d=1; d<degree; d++) {
        if (d == START_MULTITHREADING){
            break;
        }
        add_segments_simd(d, x, y);
    }
    
    //create threads
    pthread_t thread_array[THREADS];
    
    //create thread_arguments_array
    pthread_args pthread_args_arr[THREADS];
    
    for(unsigned i = 0; i < THREADS; ++i) {
        pthread_args_arr[i].x = x;
        pthread_args_arr[i].y = y;
    }

    //calculate
    for(unsigned i = START_MULTITHREADING; i < degree; ++i) {
        unsigned long long segment_length = (unsigned long long) 1 << (2 * i);
        unsigned segment_coord = 1 << i, step = segment_length/THREADS;
        
        for(unsigned j = 0; j < THREADS; ++j) {
            pthread_args_arr[j].segment_length = segment_length;
            pthread_args_arr[j].segment_coord = segment_coord;
            pthread_args_arr[j].start = j * step;
            pthread_args_arr[j].end = step + j*step;
        }
        
        for (unsigned j = 0; j < THREADS; ++j) {
            if (use_simd){
                pthread_create(&thread_array[j], NULL, add_segments_simd_multithreaded, (void *) &pthread_args_arr[j]);
            }
            else{
                pthread_create(&thread_array[j], NULL, add_segments_multithreaded, (void *) &pthread_args_arr[j]);
            }
        }
        
        for (unsigned j = 0; j < THREADS; ++j) {
            pthread_join(thread_array[j], NULL);
        }
    }
}