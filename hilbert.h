#ifndef HILBERT_H
#define HILBERT_H

typedef struct{
    coord_t *x;
    coord_t *y;
    unsigned long long segment_length;
    unsigned long long segment_coord;
    unsigned long long start;
    unsigned long long end;
}pthread_args;

typedef struct{
    coord_t *x;
    coord_t *y;
}pthread_args_temp;


void hilbert_V0(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V1(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V2(unsigned degree, coord_t* x, coord_t* y, unsigned THREADS, bool use_simd);
void hilbert_V4(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V5(unsigned degree, coord_t* x, coord_t* y);

void add_segments(unsigned degree, coord_t* x, coord_t* y);
void add_segments_simd(unsigned segment_degree, coord_t* x, coord_t* y);
void * add_segments_simd_multithreaded(void * args);
void * add_segments_multithreaded(void * args);

void v_assembly(unsigned degree, coord_t* x, coord_t* y);
void * v_assembly_multithreaded(void * args);

#endif

