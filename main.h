#ifndef V_ASSEMBLY_H
#define V_ASSEMBLY_H

#include <stddef.h>

typedef struct {
    unsigned long long val;
}coord_t;

void print_curve(unsigned degree, coord_t* x, coord_t* y);
void add_segments(unsigned degree, coord_t* x, coord_t* y);
void hilbert(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V1(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V2(unsigned degree, coord_t* x, coord_t* y);
void write_svg(char* output_file_svg, int degree, coord_t* x, coord_t* y);
void write_txt(char* output_file_txt, int degree, coord_t* x, coord_t* y);
void help_message();
double v_assembly(unsigned degree, coord_t* x, coord_t* y);

#endif
