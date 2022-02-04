#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#include "main.h"
#include "print.h"
#include "hilbert.h"
#include "test.h"

int main(int argc, char **argv) {
    
    struct timespec start, end;
    
    parameter parameter_args;
    
    parameter_args.start = &start;
    parameter_args.end = &end;
    parameter_args.messure_time = false;
    parameter_args.version = -1;
    parameter_args.count_call = 1;
    parameter_args.degree = 1;
    parameter_args.write_svg_file = false;
    parameter_args.write_txt_file = false;
    parameter_args.print_console = false;
    parameter_args.test_file = false;
    parameter_args.test_time = false;
    parameter_args.THREADS = 8;
    
      
    int option;
    int option_index = 0;
    static struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"test_file", no_argument, 0, 'a'},
            {"test_time", no_argument, 0, 'b'}
    };

    while ((option = getopt_long(argc, argv, ":V:B::n:o:u:t:hpb",long_options, &option_index)) != -1) {
        char* ptr;
        switch (option) {
            case 'V' :
                if (optarg == NULL || (*optarg != '0' && *optarg != '1' && *optarg != '2' && *optarg != '3' && *optarg != '4' && *optarg != '5' && *optarg != '6')) {
                    printf("\n\n\n\nEs stehen folgende Versionen zur verfuegung:\n\n"
                           "Default waehlt die schnellste Variante anhand vom Grad n und Anzahl der THREADS aus\n"
                           "0 --Assembler mit SIMD--\n"
                           "1 --C mit SIMD--\n"
                           "2 --C Multithreaded mit SIMD--\n"
                           "3 --C Multithreaded ohne SIMD--\n"
                           "4 --Assembler Multithreaded mit SIMD--\n"
                           "5 --C ohne Optimierung--\n"
                           "6 --Rekursiv mit SIMD--\n");
                    help_message();
                    return 0;
                }
                parameter_args.version = atoi(optarg);
                break;
            case 't' :
                ptr = optarg;
                while (*ptr) {
                    if (!isdigit(*ptr)) {
                        fprintf(stderr, "\n\n\n\nDie Anzahl an THREADS erwartet einen int und muss mindestens 1 betragen.\n\n");
                        help_message();
                        return 0;
                    }
                    ptr++;
                } 
                parameter_args.THREADS = atoi(optarg);
                if ((parameter_args.THREADS & (parameter_args.THREADS - 1 ))) {
                    //make it a potenz of two
                    int i=-1;
                    for (; parameter_args.THREADS; ++i, parameter_args.THREADS>>=1);
                    parameter_args.THREADS = 1 << i;
                    printf("p is: %d\n", parameter_args.THREADS);
                }
                if (parameter_args.THREADS > 256)
                    parameter_args.THREADS = 256;
                break;
            case 'B' :
                parameter_args.messure_time = true;
                ptr = optarg;
                if (ptr != NULL) {
                    while (*ptr) {
                        if (!isdigit(*ptr)) {
                            fprintf(stderr, "\n\n\n\nDie Eingabe -B erwartet einen positiven int.\n\n");
                            help_message();
                            return 0;
                        }
                        ptr++;
                    }
                parameter_args.count_call = atoi(optarg);
                }
                break;
            case 'n' :
                ptr = optarg;
                while (*ptr) {
                    if (!isdigit(*ptr)) {
                        fprintf(stderr, "\n\n\n\nFuer den Grad der Hilbertkurve wird ein positiver int als Eingabe erwartet.\n\n");
                        help_message();
                        return 0;
                    }
                    ptr++;
                }
                parameter_args.degree = atoi(optarg);
                break;
            case 'o' :
                if (optarg != NULL) {
                    parameter_args.write_svg_file = true;
                    parameter_args.output_file_svg = optarg;
                }
                break;
            case 'u' :
                if (optarg != NULL) {
                    parameter_args.write_txt_file = true;
                    parameter_args.output_file_txt = optarg;
                }
                break;
            case 'p' :
                parameter_args.print_console = true;
                break;
            case 'a' :
                parameter_args.test_file = true;
                break;
            case 'b' :
                parameter_args.test_time = true;
                break;
            case 'h' :
                help_message();
                return 0;
            case '?' :
                fprintf(stderr, "\n\n\n\nParameter %c nicht erkannt.\n\n", optopt);
                help_message();
                return -1;
            default :
                fprintf(stderr, "\n\n\n\nFalsche Nutzung von Parametern!\n\n");
                help_message();
                return -1;
        }
//         if (argc - optind != 0) {
//             fprintf(stderr, "Invalid Option found\n");
//             help_message();
//             return -1;
//         }
    }

    
    //prep x und y
    unsigned long long curve_length = (unsigned long long)1 << (2 * parameter_args.degree);

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

    parameter_args.x = x;
    parameter_args.y = y;
    
      
    //informationen about imput args
    print_parameter(parameter_args);

    
    //choose version to use
    if(!parameter_args.test_file)
        hilbert(parameter_args);

    //Benchmark
    if(parameter_args.test_file)
        test_func_file(parameter_args);
    
    if(parameter_args.test_time)
        test_func_time(parameter_args);
    
    //print hilbert_curve on console
    if(parameter_args.print_console)
        print_curve(parameter_args.degree, parameter_args.x, parameter_args.y);

    //save svg file
    if(parameter_args.write_svg_file) {

        create_folder("files");
        create_folder("files/svg");

        write_svg("files/svg", parameter_args.output_file_svg, parameter_args.degree, parameter_args.x, parameter_args.y);
    }
    
    //save txt file
    if(parameter_args.write_txt_file) {
        create_folder("files");
        create_folder("files/txt");
        
        write_txt("files/txt", parameter_args.output_file_txt, parameter_args.degree, parameter_args.x, parameter_args.y);
    }

    free(x);
    free(y);
    return 0;
}

void hilbert(parameter parameter_args){
    if (parameter_args.degree == 1){
        parameter_args.x[0].val = 0; parameter_args.y[0].val = 0;
        parameter_args.x[1].val = 0; parameter_args.y[1].val = 1; 
        parameter_args.x[2].val = 1; parameter_args.y[2].val = 1; 
        parameter_args.x[3].val = 1; parameter_args.y[3].val = 0;
        return;
    }
    else if(parameter_args.degree < 10 || parameter_args.THREADS > 63 || parameter_args.THREADS == 1){
        parameter_args.version = 0;
    }
    else {
        parameter_args.version = 4;
    }
    
    choose_version(parameter_args);
}

void choose_version(parameter parameter_args) {
    
    unsigned version = parameter_args.version;
    
    switch (version) {
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
        case 1:
            if (parameter_args.messure_time) {
                clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
                for (unsigned i=parameter_args.count_call; i > 0; i--)
                    hilbert_V1(parameter_args.degree, parameter_args.x, parameter_args.y);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V1(parameter_args.degree, parameter_args.x, parameter_args.y);
            }
            break;
        case 2:
            if (parameter_args.messure_time) {
                clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
                for (unsigned i=parameter_args.count_call; i > 0; i--)
                    hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS, 1);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y ,parameter_args.THREADS, 1);
            }
            break;
        case 3:
            if (parameter_args.messure_time) {
                clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
                for (unsigned i=parameter_args.count_call; i > 0; i--)
                    hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS, 0);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS, 0);
            }
            break;
        case 4:
            if (parameter_args.messure_time) {
                clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
                for (unsigned i=parameter_args.count_call; i > 0; i--)
                    hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
            }
            break;
        case 5:
            if (parameter_args.messure_time) {
                clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
                for (unsigned i=parameter_args.count_call; i > 0; i--)
                    hilbert_V5(parameter_args.degree, parameter_args.x, parameter_args.y);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V5(parameter_args.degree, parameter_args.x, parameter_args.y);
            }
            break;
        case 6:
            if (parameter_args.messure_time) {
                clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
                for (unsigned i=parameter_args.count_call; i > 0; i--)
                    hilbert_V6(parameter_args.degree, parameter_args.x, parameter_args.y);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V6(parameter_args.degree, parameter_args.x, parameter_args.y);
            }
            break;
        default :
            help_message();
    }
    if(parameter_args.messure_time){
        printf("Zeitmessung ergibt: ");
        print_time(*parameter_args.start, *parameter_args.end);
    }
}
