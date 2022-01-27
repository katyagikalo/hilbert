#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <xmmintrin.h>
#include <unistd.h>
#include <pthread.h>

#include "main.h"
#include "print.h"
#include "hilbert.h"


void print_time(struct timespec *start, struct timespec *end);
void choose_version(parameter parameter_args);

int main(int argc, char **argv) {
    int version = 0;
    bool print_console,messure_time,write_txt_file,write_svg_file, test_all;
    print_console=messure_time=write_svg_file=write_txt_file=test_all = false;
    char *output_file_svg, *output_file_txt;
    int degree = 1;
    int count_call = 1;
    //clock_t start, end;
    
    int option;
    int option_index = 0;
    static struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"test_all", no_argument, 0, 'b'}
    };

    while ((option = getopt_long(argc, argv, ":V:B::n:o:t:hp",long_options, &option_index)) != -1) {
        char* ptr;
        switch (option) {
            case 'V' :
                if (optarg == NULL || (*optarg != '0' && *optarg != '1' && *optarg != '2' && *optarg != '3' && *optarg != '4' && *optarg != '5')) {
                    printf("\n\n\n\nEs stehen folgende Versionen zur verfuegung:\n\n"
                                    "0 --C ohne Optimierung--\n"
                                    "1 --Assembler mit SIMD--\n"
                                    "2 --C mit SIMD--\n"
                                    "3 --Rekursiv mit SIMD--\n"
                                    "4 --C Multithreaded mit SIMD--\n"
                                    "5 --C Multithreaded ohne SIMD--");
                    
                    help_message();
                    return 0;
                }
                version = atoi(optarg);
                break;
            case 'B' :
                messure_time = true;
                if (optarg != NULL)
                    count_call = atoi(optarg);
                break;
            case 'n' :
                ptr = optarg;
                while (*ptr) {
                    if (!isdigit(*ptr)) {
                        printf("\n\n\n\nFuer den Grad der Hilbertkurve wird ein int als Eingabe erwartet.\n");
                        help_message();
                        return 0;
                    }
                    ptr++;
                }
                degree = atoi(optarg);
                break;
            case 'o' :
                if (optarg != NULL) {
                    write_svg_file = true;
                    output_file_svg = optarg;
                }
                break;
            case 't' :
                if (optarg != NULL) {
                    write_txt_file = true;
                    output_file_txt = optarg;
                }
                break;
            case 'p' :
                print_console = true;
                break;
            case 'b' :
                test_all = true;
                break;
            case 'h' :
                help_message();
                return 0;
            case '?' :
                fprintf(stderr, "\n\n\n\nParameter %c nicht erkannt.\n\n", optopt);
                help_message();
                return 0;
            default :
                fprintf(stderr, "\n\n\n\nFalsche Nutzung von Parametern!\n\n");
                help_message();
                return 0;
        }
    }

    
    //prep x und y
    unsigned long long curve_length = (unsigned long long)1 << (2 * degree);

    coord_t *x;

    x = malloc(sizeof(coord_t)*curve_length);
    if(x == NULL){
        printf("x was null\n");
        return -1;
    }

    coord_t *y;

    y = malloc(sizeof(coord_t)*curve_length);
    if(y == NULL){
        printf("y was null\n");
        return -1;
    }
    
    parameter parameter_args;
    parameter_args.degree = degree;
    parameter_args.version = version;
    parameter_args.messure_time = messure_time;
    parameter_args.write_svg_file = write_svg_file;
    parameter_args.write_txt_file = write_txt_file;
    parameter_args.print_console = print_console;
    parameter_args.count_call = count_call;
    parameter_args.output_file_svg = output_file_svg;
    parameter_args.output_file_txt = output_file_txt;
    parameter_args.x = x;
    parameter_args.y = y;

    
//informationen ueber die Optionen und Parametereingabe
    print_parameter(parameter_args);

    //choose_version(parameter_args);
    hilbert_V0(parameter_args.degree, parameter_args.x, parameter_args.y);
    if(print_console)
        print_curve(degree, x, y);

//messure time
    if(messure_time){
        print_time(parameter_args.start, parameter_args.end);
        //printf("\n\n\n\nGenutzte Prozessorzeit: %f Sekunden\n\n\n\n",((double) (end - start)) / CLOCKS_PER_SEC);
    }
//save svg file
    if(write_svg_file)
        write_svg(output_file_svg, degree, x, y);
//save txt file
    if(write_txt_file)
        write_txt(output_file_txt, degree, x, y);

    free(x);
    free(y);
    return 0;
}

void choose_version(parameter parameter_args) {
    switch (parameter_args.version) {
        case 0:
            if (parameter_args.messure_time) {
                clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
                for (unsigned i=parameter_args.count_call; i > 0; i--)
                    hilbert_V0(parameter_args.degree, parameter_args.x, parameter_args.y);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V0(parameter_args.degree, parameter_args.x, parameter_args.y);
            }
            break;
//         case 1:
//             if (parameter_args.messure_time) {
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.start);
//                 for (unsigned i=parameter_args.count_call; i > 0; i--)
//                     hilbert_V1(parameter_args.degree, parameter_args.x, parameter_args.y);
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.end);
//             }
//             else {
//                 hilbert_V1(parameter_args.degree, parameter_args.x, parameter_args.y);
//             }
//             break;
//         case 2:
//             if (parameter_args.messure_time) {
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.start);
//                 for (unsigned i=parameter_args.count_call; i > 0; i--)
//                     hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y);
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.end);
//             }
//             else {
//                 hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y);
//             }
//             break;
//         case 3:
//             if (parameter_args.messure_time) {
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.start);
//                 for (unsigned i=parameter_args.count_call; i > 0; i--)
//                     hilbert_V3(parameter_args.degree, parameter_args.x, parameter_args.y);
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.end);
//             }
//             else {
//                 hilbert_V3(parameter_args.degree, parameter_args.x, parameter_args.y);
//             }
//             break;
//         case 4:
//             if (parameter_args.messure_time) {
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.start);
//                 for (unsigned i=parameter_args.count_call; i > 0; i--)
//                     hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y);
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.end);
//             }
//             else {
//                 hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y);
//             }
//             break;
//             case 5:
//             if (parameter_args.messure_time) {
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.start);
//                 for (unsigned i=parameter_args.count_call; i > 0; i--)
//                     hilbert_V5(parameter_args.degree, parameter_args.x, parameter_args.y);
//                 clock_gettime(CLOCK_MONOTONIC, &parameter_args.end);
//             }
//             else {
//                 hilbert_V5(parameter_args.degree, parameter_args.x, parameter_args.y);
//             }
//             break;
        default :
            help_message();
    }
}

void print_time(struct timespec *start, struct *timespec end) {
     double elapsed = (end->tv_sec - start->tv_sec);
     elapsed += (end->tv_nsec - start->tv_nsec) / 1000000000.0;
     printf("Zeitmessung ergibt: %f Sekunden\n", elapsed);
}
