#ifndef PRINT_H
#define PRINT_H

void print_parameter(parameter parameter_args);
void print_curve(unsigned degree, coord_t* x, coord_t* y);
void print_time(struct timespec *start, struct timespec *end);
void write_svg(char* output_file_svg, int degree, coord_t* x, coord_t* y);
void write_txt(char* output_file_txt, int degree, coord_t* x, coord_t* y);
void help_message();

#endif
