#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <xmmintrin.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "main.h"
#include "print.h"
#include "hilbert.h"

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
    parameter_args.test_all = false;
    parameter_args.test_time = false;
    parameter_args.THREADS = 8;
    
      
    int option;
    int option_index = 0;
    static struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"test_all", no_argument, 0, 'a'},
            {"test_time", no_argument, 0, 'b'}
    };

    while ((option = getopt_long(argc, argv, ":V:B::n:o:u:t:hpb",long_options, &option_index)) != -1) {
        char* ptr;
        switch (option) {
            case 'V' :
                if (optarg == NULL || (*optarg != '0' && *optarg != '1' && *optarg != '2' && *optarg != '3' && *optarg != '4' && *optarg != '5' && *optarg != '6')) {
                    printf("\n\n\n\nEs stehen folgende Versionen zur verfuegung:\n\n"
                                    "Default waehlt die schnellste Variante anhand vom Grad n aus\n"
                                    "0 --Assembler mit SIMD--\n"
                                    "1 --C mit SIMD--\n"
                                    "2 --C Multithreaded mit SIMD--\n"
                                    "3 --C Multithreaded ohne SIMD--\n"
                                    "4 --C ohne Optimierung--\n"
                                    "5 --Rekursiv mit SIMD--\n");
                    help_message();
                    return 0;
                }
                parameter_args.version = atoi(optarg);
                break;
            case 't' :
                ptr = optarg;
                while (*ptr) {
                    if (!isdigit(*ptr)) {
                        printf("\n\n\n\nDie Anzahl an THREADS erwartet einen int und muss mindestens 1 betragen.\n");
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
                break;    
            case 'B' :
                parameter_args.messure_time = true;
                ptr = optarg;
                while (*ptr) {
                    if (!isdigit(*ptr)) {
                        printf("\n\n\n\nDie Eingabe -B erwartet einen positiven int.\n");
                        help_message();
                        return 0;
                    }
                    ptr++;
                }
                    parameter_args.count_call = atoi(optarg);
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
                parameter_args.test_all = true;
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
    if(!parameter_args.test_all)
        choose_version(parameter_args);

    //Benchmark
    if(parameter_args.test_all || parameter_args.test_time)
        test_func(parameter_args);
    
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

void choose_version(parameter parameter_args) {
    
    unsigned version = parameter_args.version;
    
    //default choosing algorithm
    if (parameter_args.version == -1) {
        if (parameter_args.degree < 12)
            version = 0;
        else 
            version = 2;
    }
    
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
            //outcomment later
            printf("this was v_assembly\n\n");
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
            //outcomment later
            printf("this was multithreaded\n\n");
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
                    hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y);
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
                    hilbert_V6(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
                clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
            }
            else {
                hilbert_V6(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
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

void test_func(parameter parameter_args){

    if (parameter_args.test_all) {
        create_folder("test_files");
        create_folder("test_files/svg");
        create_folder("test_files/txt");
    }
    
    //Version 0
    clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
    hilbert_V0(parameter_args.degree, parameter_args.x, parameter_args.y);
    clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
    printf("Version 0 ----Assembler mit SIMD----------- : ");
    print_time(*parameter_args.start, *parameter_args.end);
    if (parameter_args.test_all) {
        write_svg("test_files/svg", "Version_0", parameter_args.degree, parameter_args.x, parameter_args.y);
        write_txt("test_files/txt", "Version_0", parameter_args.degree, parameter_args.x, parameter_args.y);
    }
    
    //Version 1
    clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
    hilbert_V1(parameter_args.degree, parameter_args.x, parameter_args.y);
    clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
    printf("Version 1 ----C mit SIMD------------------- : ");
    print_time(*parameter_args.start, *parameter_args.end);
    if (parameter_args.test_all) {
        write_svg("test_files/svg", "Version_1", parameter_args.degree, parameter_args.x, parameter_args.y);
        write_txt("test_files/txt", "Version_1", parameter_args.degree, parameter_args.x, parameter_args.y);
    }
    
    //Version 2
    clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
    hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS, 1);
    clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
    printf("Version 2 ----C Multithreaded mit SIMD----- : ");
    print_time(*parameter_args.start, *parameter_args.end);
    if (parameter_args.test_all) {
        write_svg("test_files/svg", "Version_2", parameter_args.degree, parameter_args.x, parameter_args.y);
        write_txt("test_files/txt", "Version_2", parameter_args.degree, parameter_args.x, parameter_args.y);
    }
    
    //Version 3
    clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
    hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS, 0);
    clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
    printf("Version 3 ----C Multithreaded ohne SIMD---- : ");
    print_time(*parameter_args.start, *parameter_args.end);
    if (parameter_args.test_all) {
        write_svg("test_files/svg", "Version_3", parameter_args.degree, parameter_args.x, parameter_args.y);
        write_txt("test_files/txt", "Version_3", parameter_args.degree, parameter_args.x, parameter_args.y);
    }
    
    //Version 4
    clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
    hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y);
    clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
    printf("Version 4 ----C ohne Optimierung----------- : ");
    print_time(*parameter_args.start, *parameter_args.end);
    if (parameter_args.test_all) {
        write_svg("test_files/svg", "Version_4", parameter_args.degree, parameter_args.x, parameter_args.y);
        write_txt("test_files/txt", "Version_4", parameter_args.degree, parameter_args.x, parameter_args.y);
    }
    
    //Version 5
    clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
    hilbert_V5(parameter_args.degree, parameter_args.x, parameter_args.y);
    clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
    printf("Version 5 ----C Rekursiv mit SIMD --------- : ");
    print_time(*parameter_args.start, *parameter_args.end);
    if (parameter_args.test_all) {
        write_svg("test_files/svg", "Version_5", parameter_args.degree, parameter_args.x, parameter_args.y);
        write_txt("test_files/txt", "Version_5", parameter_args.degree, parameter_args.x, parameter_args.y);
    }
}
