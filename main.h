#ifndef V_ASSEMBLY_H
#define V_ASSEMBLY_H

#include <stddef.h>

typedef struct {
    unsigned val;
}coord_t;

typedef struct {
    int degree, version, count_call;
    bool messure_time, write_svg_file, write_txt_file, print_console;
    char *output_file_svg, *output_file_txt;
    struct timespec start, end;
    coord_t *x, *y;
}parameter;

double v_assembly(unsigned degree, coord_t x, coord_t* y);

#endif
