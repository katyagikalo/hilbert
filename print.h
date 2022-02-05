#ifndef PRINT_H
#define PRINT_H

void print_parameter(parameter parameter_args);
void print_curve(unsigned degree, coord_t* x, coord_t* y);
double print_time(struct timespec start, struct timespec end);
void print_result(unsigned degree, double result[][6]);
void create_folder(char path[]);
void write_svg(char *path, char *output_file_svg, int degree, coord_t* x, coord_t* y);
void write_txt(char *path, char* output_file_txt, int degree, coord_t* x, coord_t* y);
void help_message();

#endif
