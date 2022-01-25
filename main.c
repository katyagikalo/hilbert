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

void print_curve(unsigned degree, coord_t* x, coord_t* y);
void add_segments(unsigned degree, coord_t* x, coord_t* y);
void add_segments_simd(unsigned segment_degree, coord_t* x, coord_t* y);
void hilbert_recursive(unsigned degree, unsigned i, coord_t* x, coord_t* y);
void * add_segment_left_upper(void * pthread_args);
void * add_segment_right_upper(void * pthread_args);
void * add_segment_left_lower(void * pthread_args);
void * add_segment_right_lower(void * pthread_args);
void hilbert_V0(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V1(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V2(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V3(unsigned i, coord_t* x, coord_t* y);
void hilbert_V4(unsigned degree, coord_t* x, coord_t* y);
void write_svg(char* output_file_svg, int degree, coord_t* x, coord_t* y);
void write_txt(char* output_file_txt, int degree, coord_t* x, coord_t* y);
void help_message();

typedef struct{
    unsigned long long segment_length;
    unsigned segment_coord;
    coord_t *x;
    coord_t *y;
}pthread_args;

int main(int argc, char **argv) {
    int version = 0;
    bool print_console,messure_time,write_txt_file,write_svg_file;
    print_console=messure_time=write_svg_file=write_txt_file = false;
    char *output_file_svg, *output_file_txt;
    int degree = 1;
    int count_call = 1;
    clock_t start, end;

    int option;
    int option_index = 0;
    static struct option long_options[] = {
            {"help", no_argument, 0, 'h'}
    };

    while ((option = getopt_long(argc, argv, ":V:B::n:o:t:hp",long_options, &option_index)) != -1) {
        char* ptr;
        switch (option) {
            case 'V' :
                if (optarg == NULL || (*optarg != '0' && *optarg != '1' && *optarg != '2' && *optarg != '3' && *optarg != '4')) {
                    printf("\n\n\n\nEs stehen folgende Versionen zur verfuegung:\n\n0 --C ohne Optimierung--\n1 --Assembler ohne SIMD--\n2 --C mit SIMD--\n3 --Rekursiv mit SIMD--\n4 --C Multithreaded ohne SIMD--\n");
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

//informationen ueber die Optionen und Parametereingabe
    printf("\n\n"
           "Version                  : V%d\n"
           "Grad der Hilbertkurve    : %d\n"
           "Zeitmessung              : %s\n"
           "Funktionsaufrufe         : %d\n"
           "AusgabeArray auf Konsole : %s\n"
           "SVG_Ausgabedatei         : %s\n"
           "TXT_Ausgabedatei         : %s\n",
           version, degree, (messure_time ? "ja" : "nein"), count_call,
           (print_console ? "ja" : "nein"),
           (write_svg_file ? output_file_svg : "--nicht gewaehlt--"),
           (write_txt_file ? output_file_txt : "--nicht gewaehlt--"));

//prep x und y
    unsigned long long curve_length = (unsigned long long)1 << (2 * degree);

    coord_t* x;

    x = malloc(sizeof(coord_t)*curve_length);
    if(x == NULL){
        printf("x was null\n");
        return -1;
    }

    coord_t* y;

    y = malloc(sizeof(coord_t)*curve_length);
    if(y == NULL){
        printf("y was null\n");
        return -1;
    }

//hilbert
    switch (version) {
        case 0:
            if(messure_time) {
                start = clock();
                for (unsigned i=count_call; i > 0; i--)
                    hilbert_V0(degree, x, y);
                end = clock();
            }
            else {
                hilbert_V0(degree, x, y);
            }
            break;
        case 1:
            if(messure_time) {
                start = clock();
                for (unsigned i=count_call; i > 0; i--)
                    hilbert_V1(degree, x, y);
                end = clock();
            }
            else {
                hilbert_V1(degree, x, y);
            }
            break;
        case 2:
            if(messure_time) {
                start = clock();
                for (unsigned i=count_call; i > 0; i--)
                    hilbert_V2(degree, x, y);
                end = clock();
            }
            else {
                hilbert_V2(degree, x, y);
            }
            break;
        case 3:
            if(messure_time) {
                start = clock();
                for (unsigned i=count_call; i > 0; i--)
                    hilbert_V3(degree, x, y);
                end = clock();
            }
            else {
                hilbert_V3(degree, x, y);
            }
            break;
        case 4:
            if(messure_time) {
                start = clock();
                for (unsigned i=count_call; i > 0; i--)
                    hilbert_V4(degree, x, y);
                end = clock();
            }
            else {
                hilbert_V4(degree, x, y);
            }
            break;
        default :
            help_message();

            return 0;
    }

    if(print_console)
        print_curve(degree, x, y);

//messure time
    if(messure_time)
        printf("\n\n\n\nZeitmessung ergibt: %f Sekunden\n\n\n\n",((double) (end - start)) / CLOCKS_PER_SEC);

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





void add_segments(unsigned segment_degree, coord_t* x, coord_t* y){
    unsigned long long segment_length = (unsigned long long)1 << (2 * (segment_degree));
    unsigned segment_coord = (1 << segment_degree);
    
    //2*segment_length
    unsigned long long d_segment_length = segment_length + segment_length;
    //3*segment_length
    unsigned long long t_segment_length = d_segment_length + segment_length;
    
    for(unsigned long long i = 0; i < segment_length; ++i) {
        //left upper segment
        x[segment_length + i].val = x[i].val;
        y[segment_length + i].val = y[i].val + segment_coord;

        //right upper segment
        x[d_segment_length].val = x[i].val + segment_coord;
        y[d_segment_length].val = y[i].val + segment_coord;

        //left lower segment
        unsigned temp = x[i].val;
        x[i].val = y[i].val;
        y[i].val = temp;

        //right lower segment
        x[t_segment_length + i].val = 2*segment_coord - 1 - x[i].val;
        y[t_segment_length + i].val = segment_coord - 1 - y[i].val;
    }
}


void hilbert_V0(unsigned degree, coord_t* x, coord_t* y) {
    //curve for degree = 1
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    for(unsigned i = 1; i < degree; ++i){
        add_segments(i, x, y);
    }
}


void hilbert_V1(unsigned degree, coord_t* x, coord_t* y) {
    v_assembly(degree, x, y);
}

void add_segments_simd(unsigned segment_degree, coord_t* x, coord_t* y){
    unsigned long long segment_length = (unsigned long long)1 << (2 * (segment_degree));
    unsigned segment_coord = (1 << segment_degree);

    coord_t *vx = x, *vy = y;
    
    //2*segment_length
    unsigned long long d_segment_length = segment_length + segment_length;
    //3*segment_length
    unsigned long long t_segment_length = d_segment_length + segment_length;
    
    __m128i arr_x, arr_y;
    
    __m128i sc = _mm_set1_epi32(segment_coord);
    __m128i d_sc = _mm_add_epi32(sc, sc);
    __m128i one = _mm_set1_epi32(1);
    
    for(unsigned long long i = 0; i < segment_length; i+=4) {

        arr_x = _mm_loadu_si128((__m128i const*)(vx));
        arr_y = _mm_loadu_si128((__m128i const*)(vy));

        //left upper segment
        _mm_storeu_si128((__m128i*)(vx + segment_length), arr_x);
        _mm_storeu_si128((__m128i*)(vy + segment_length), _mm_add_epi32(arr_y, sc));

        //right upper segment
        _mm_storeu_si128((__m128i*)(vx + d_segment_length), _mm_add_epi32(arr_x, sc));
        _mm_storeu_si128((__m128i*)(vy + d_segment_length), _mm_add_epi32(arr_y, sc));

        //left lower segment
        _mm_storeu_si128((__m128i*)(vx), arr_y);
        _mm_storeu_si128((__m128i*)(vy), arr_x);

        //right lower segment
        _mm_storeu_si128((__m128i*)(vx + t_segment_length), _mm_sub_epi32(_mm_sub_epi32(d_sc, one), arr_y));
        _mm_storeu_si128((__m128i*)(vy + t_segment_length), _mm_sub_epi32(_mm_sub_epi32(sc, one), arr_x));
        
        vx+=4;
        vy+=4;
    }
}

void hilbert_V2(unsigned degree, coord_t* x, coord_t* y){
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    for(unsigned i = 1; i < degree; ++i){
        add_segments_simd(i, x, y);
    }
}

void hilbert_recursive(unsigned degree, unsigned i, coord_t* x, coord_t* y){
    if (degree == i) return;

    else {
        add_segments_simd(i, x, y);
    }
    hilbert_recursive(degree, i+1, x, y);
}


void hilbert_V3(unsigned degree, coord_t* x, coord_t* y){
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    unsigned i = 1;

    hilbert_recursive(degree, i, x, y);
}



void * add_segment_left_upper(void * args){
    pthread_args* temp_args = (pthread_args*) args;
    for(unsigned long long i = 0; i < temp_args->segment_length; ++i) {
        //left upper segment
        temp_args->x[temp_args->segment_length + i].val = temp_args->x[i].val;
        temp_args->y[temp_args->segment_length + i].val = temp_args->y[i].val + temp_args->segment_coord;
    }
    return NULL;
}

void * add_segment_right_upper(void * args){
    pthread_args* temp_args = (pthread_args*) args;
    for(unsigned long long i = 0; i < temp_args->segment_length; ++i) {
        //right upper segment
        temp_args->x[2*temp_args->segment_length + i].val = temp_args->x[i].val + temp_args->segment_coord;
        temp_args->y[2*temp_args->segment_length + i].val = temp_args->y[i].val + temp_args->segment_coord;
    }
    return NULL;
}

void * add_segment_left_lower(void * args){
    pthread_args* temp_args = (pthread_args*) args;
    for(unsigned long long i = 0; i < temp_args->segment_length; ++i) {
        //left lower segment
        unsigned temp = temp_args->x[i].val;
        temp_args->x[i].val = temp_args->y[i].val;
        temp_args->y[i].val = temp;
    }
    return NULL;
}

void * add_segment_right_lower(void * args){
    pthread_args* temp_args = (pthread_args*) args;
    for(unsigned long long i = 0; i < temp_args->segment_length; ++i) {
        //right lower segment
        temp_args->x[3*temp_args->segment_length + i].val = 2*temp_args->segment_coord - 1 - temp_args->x[i].val;
        temp_args->y[3*temp_args->segment_length + i].val = temp_args->segment_coord - 1 - temp_args->y[i].val;
    }
    return NULL;
}

void hilbert_V4(unsigned degree, coord_t* x, coord_t* y){
    //curve for degree = 1
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    pthread_t tid0;
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    pthread_args pthread_args_arr[degree + 1];

    for(unsigned i = 1; i < degree; ++i){
        pthread_args_arr[i-1].segment_length=(unsigned long long)1 << (2 * i);
        pthread_args_arr[i-1].segment_coord = (1 << i);
        pthread_args_arr[i-1].x=x;
        pthread_args_arr[i-1].y=y;

        pthread_create(&tid0, NULL, add_segment_left_upper, (void *) &pthread_args_arr[i-1]);
        pthread_create(&tid1, NULL, add_segment_right_upper, (void *) &pthread_args_arr[i-1]);
        pthread_create(&tid2, NULL, add_segment_left_lower, (void *) &pthread_args_arr[i-1]);
        pthread_create(&tid3, NULL, add_segment_right_lower, (void *) &pthread_args_arr[i-1]);

        pthread_join(tid0,NULL);
        pthread_join(tid1,NULL);
        pthread_join(tid2,NULL);
        pthread_join(tid3,NULL);
    }
}

void print_curve(unsigned degree, coord_t* x, coord_t* y){
    unsigned long long length = (unsigned long long)1 << (2*degree);
    printf("\n\n\n\n\nArray der Koordinaten:\n\n");
    for(unsigned long long i = 0; i < length; ++i) {
        printf("(%d,%d) ", x[i].val, y[i].val);
    }
    printf("\n");
}

void write_svg(char* output_file_svg, int degree, coord_t* x, coord_t* y) {
    unsigned long long length = (unsigned long long)1 << (2*degree);
    unsigned win_size = 1 << degree;

    char file_name[strlen(output_file_svg) + 5];
    FILE* svg_fp = fopen(strcat(strcpy(file_name, output_file_svg),".svg\0"),"w");

    if (svg_fp == NULL) {
        printf("File kann nicht angelegt werden\n");
        return;
    }

    fprintf(svg_fp, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\" ?>\n"
                    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\"\n"
                    "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n"
                    "<svg width=\"%d0\" height=\"%d0\" xmlns=\"http://www.w3.org/2000/svg\"\n"
                    "xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
                    "<title>Polyline-Element</title>\n"
                    "<desc>Polylinie</desc>\n"
                    "<polyline fill=\"none\" stroke=\"black\" stroke-width=\"1px\"\n"
                    "points=\"00 00", win_size, win_size);

    for (unsigned long long i = 1; i < length; ++i)
        fprintf(svg_fp, ",%d0 %d0", x[i].val, y[i].val);

    fprintf(svg_fp,"\" transform=\"scale(1 -1) translate(0 -%d0)\"/>\n</svg>\n",win_size);
    fclose(svg_fp);
}


void write_txt(char* output_file_txt, int degree, coord_t* x, coord_t *y) {
    unsigned long long length = (unsigned long long)1 << (2*degree);

    char file_name[strlen(output_file_txt) + 5];
    FILE* txt_fp = fopen(strcat(strcpy(file_name, output_file_txt),".txt\0"),"w");

    if (txt_fp == NULL) {
        printf("File kann nicht angelegt werden\n");
        return;
    }
    for (unsigned long long i = 0; i < length; ++i)
        fprintf(txt_fp, "(%d,%d)", x[i].val, y[i].val);
    fclose(txt_fp);
}


void help_message() {
    printf("\nDefaultwerte sind: -V0 -n1\n\n"
           "./main [-V<int>] [-B<int>] [-n<int>] [-o<file>] [-h] [--help]\n\n"
           "Options:\n"
           "  -V<int>        --Version                                <int>   -> V0 bis V1\n"
           "  -B<int>[opt]   --Laufzeitmessung                        <int>   -> Anzahl Aufrufe\n"
           "  -n<int>        --Grad der Hilbertkurve                  <int>   -> Grad der Hilbertkurve\n"
           "  -p             --Koordinatenausgabe auf Konsole\n"
           "  -o<file>       --SVG Ausgabedatei                       <file>  -> Name der SVG Ausgabedatei\n"
           "  -t<file)       --txt Ausgabedatei                       <file>  -> Name der txt Ausgabedatei\n"
           "  -h             --Hilfe\n"
           "  --help         --Hilfe\n\n\n"
           "Beispielaufrufe:   ./main -h\n"
           "                   ./main --help\n"
           "                   ./main\n"
           "                   ./main -B -n4\n"
           "                   ./main -o file_name -n5\n"
           "                   ./main -V1 -B5 -n3 -o file_name\n"
           "                   ./main -V0 -B12 -n4 -o my_hilbert_svg -t my_hilbert_txt\n\n\n"
           "Wird eine Option mit Optionsargument doppelt aufgerufen, so wird das letzte Optionsargument verwendet.\n\n\n\n");
}
