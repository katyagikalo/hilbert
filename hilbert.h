#ifndef HILBERT_H
#define HILBERT_H

typedef struct{
    coord_t *x;
    coord_t *y;
    unsigned long long segment_length;
    unsigned long long start;
    unsigned long long end;
    unsigned segment_coord;
}pthread_args;

void hilbert_V0(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V1(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V2(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V3(unsigned degree, coord_t* x, coord_t* y, unsigned THREADS);
void hilbert(unsigned degree, coord_t* x, coord_t* y, unsigned THREADS);
void hilbert_V5(unsigned degree, coord_t* x, coord_t* y, unsigned THREADS);

void add_segments(unsigned degree, coord_t* x, coord_t* y);
void add_segments_simd(unsigned segment_degree, coord_t* x, coord_t* y);
void * add_segments_simd_multithreaded(void * args);
void * add_segments_multithreaded(void * args);

void v_assembly(unsigned degree, coord_t* x, coord_t* y);
void * v_assembly_multithreaded(void * args);

#endif

