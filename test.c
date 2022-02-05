#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "hilbert.h"
#include "print.h"
#include "test.h"

void test_func_file(parameter parameter_args){
    create_folder("test_files");
    create_folder("test_files/svg");
    create_folder("test_files/txt");
    
    hilbert(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
    write_svg("test_files/svg", "Version_0", parameter_args.degree, parameter_args.x, parameter_args.y);
    write_txt("test_files/txt", "Version_0", parameter_args.degree, parameter_args.x, parameter_args.y);
    
    hilbert_V1(parameter_args.degree, parameter_args.x, parameter_args.y);
    write_svg("test_files/svg", "Version_1", parameter_args.degree, parameter_args.x, parameter_args.y);
    write_txt("test_files/txt", "Version_1", parameter_args.degree, parameter_args.x, parameter_args.y);
    
    hilbert_V2(parameter_args.degree, parameter_args.x, parameter_args.y);
    write_svg("test_files/svg", "Version_2", parameter_args.degree, parameter_args.x, parameter_args.y);
    write_txt("test_files/txt", "Version_2", parameter_args.degree, parameter_args.x, parameter_args.y);
    
    hilbert_V3(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
    write_svg("test_files/svg", "Version_3", parameter_args.degree, parameter_args.x, parameter_args.y);
    write_txt("test_files/txt", "Version_3", parameter_args.degree, parameter_args.x, parameter_args.y);
    
    hilbert_V4(parameter_args.degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
    write_svg("test_files/svg", "Version_4", parameter_args.degree, parameter_args.x, parameter_args.y);
    write_txt("test_files/txt", "Version_4", parameter_args.degree, parameter_args.x, parameter_args.y);
    
    hilbert_V5(parameter_args.degree, parameter_args.x, parameter_args.y);
    write_svg("test_files/svg", "Version_5", parameter_args.degree, parameter_args.x, parameter_args.y);
    write_txt("test_files/txt", "Version_5", parameter_args.degree, parameter_args.x, parameter_args.y);
}

void test_func_time(parameter parameter_args){
    
    double result[parameter_args.degree][7];
    
    for (unsigned temp_degree = 1; temp_degree <= parameter_args.degree; temp_degree++) {

        
        printf("n=%d\n", temp_degree);
        //Default Version
        clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
        hilbert(temp_degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
        clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
        printf("Version 0 ----Assembler mit SIMD------------------ : ");
        result[temp_degree-1][0] = print_time(*parameter_args.start, *parameter_args.end);
        
        //Version 1
        clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
        hilbert_V1(temp_degree, parameter_args.x, parameter_args.y);
        clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
        printf("Version 1 ----C mit SIMD-------------------------- : ");
        result[temp_degree-1][1] = print_time(*parameter_args.start, *parameter_args.end);
        
        //Version 2
        clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
        hilbert_V2(temp_degree, parameter_args.x, parameter_args.y);
        clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
        printf("Version 2 ----C Multithreaded mit SIMD------------ : ");
        result[temp_degree-1][2] = print_time(*parameter_args.start, *parameter_args.end);
        
        //Version 3
        clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
        hilbert_V3(temp_degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
        clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
        printf("Version 3 ----C Multithreaded ohne SIMD----------- : ");
        result[temp_degree-1][3] = print_time(*parameter_args.start, *parameter_args.end);
        
        //Version 4
        clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
        hilbert_V4(temp_degree, parameter_args.x, parameter_args.y, parameter_args.THREADS);
        clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
        printf("Version 4 ----Assembler Multithreaded mit SIMD --- : ");
        result[temp_degree-1][4] = print_time(*parameter_args.start, *parameter_args.end);
        
        //Version 5
        clock_gettime(CLOCK_MONOTONIC, parameter_args.start);
        hilbert_V5(temp_degree, parameter_args.x, parameter_args.y);
        clock_gettime(CLOCK_MONOTONIC, parameter_args.end);
        printf("Version 5 ----C Rekursiv mit SIMD ---------------- : ");
        result[temp_degree-1][5] = print_time(*parameter_args.start, *parameter_args.end);
    }
    print_result(parameter_args.degree, result);
}

