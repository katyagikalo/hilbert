#ifndef HILBERT_H
#define HILBERT_H

typedef struct{
    coord_t *x;
    coord_t *y;
    unsigned long long segment_length;
    unsigned segment_coord;
    unsigned start;
    unsigned end;
}pthread_args;

void hilbert_V0(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V1(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V2(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V3(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V4(unsigned degree, coord_t* x, coord_t* y, bool use_simd);

void add_segments(unsigned degree, coord_t* x, coord_t* y);
void add_segments_simd(unsigned segment_degree, coord_t* x, coord_t* y);
void * add_segments_simd_multithreaded(void * args);
void * add_segments_multithreaded(void * args);
void * v_assembly(pthread_args * args);

#endif

