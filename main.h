#ifndef MAIN_H
#define MAIN_H

typedef struct {
    unsigned val;
}coord_t;

typedef struct {
    unsigned degree, version, count_call, THREADS;
    bool messure_time, write_svg_file, write_txt_file, print_console, test_file, test_time;
    char *output_file_svg, *output_file_txt;
    struct timespec *start, *end;
    coord_t *x, *y;
}parameter;

void hilbert(parameter parameter_args);
void choose_version(parameter parameter_args);
void test_func_time(parameter parameter_args);
void test_func_file(parameter parameter_args);

#endif
