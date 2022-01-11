#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "main.h"

void print_curve(unsigned degree, coord_t* x, coord_t* y);
void add_segments(unsigned degree, coord_t* x, coord_t* y);
void hilbert(unsigned degree, coord_t* x, coord_t* y);
void hilbert_V1(unsigned degree, coord_t* x, coord_t* y);
void write_svg(char* output_file_svg, int degree, coord_t* x, coord_t* y);
void write_txt(char* output_file_txt, int degree, coord_t* x, coord_t* y);
void help_message();

int main(int argc, char **argv) {
    int version = 0;
    bool print_console,messure_time,write_txt_file,write_svg_file;
    print_console=messure_time=write_svg_file=write_txt_file = false;
    char *output_file_svg;
    char *output_file_txt;
    int degree = 1;
    int count_call = 1;
    clock_t start, end;

    coord_t* x;
    coord_t* y;

    int option;
    int option_index = 0;
    static struct option long_options[] = {
            {"help", no_argument, 0, 'h'}
    };
    while ((option = getopt_long(argc, argv, ":V:B::n:o:t:hp",long_options, &option_index)) != -1) {
        char* ptr;
        switch (option) {
            case 'V' :
                if (optarg == NULL || (*optarg != '0' && *optarg != '1')) {
                    printf("\n\n\n\nFuer die Version sind nur Version 0(deault) and 1(not implemented yet) verfuegbar\n\n");
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
    printf("\n\n\n\n\n"
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
    unsigned curve_length = 1 << (2 * degree);
    x = malloc(sizeof(coord_t)*curve_length);
    y = malloc(sizeof(coord_t)*curve_length);
    
    
    x2 = malloc(sizeof(coord_t)*curve_length);
    y2 = malloc(sizeof(coord_t)*curve_length);
    
    hilbert(degree, x, y);
    hilbert_V1(degree, x2, y2);
    
    compare(degree, x, y, x2, y2);

//hilbert
    switch (version) {
        case 0:
            if(messure_time) {
                start = clock();
                for (unsigned i=count_call; i > 0; i--)
                    hilbert(degree, x, y);
                end = clock();
            }
            else {
                hilbert(degree, x, y);
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

    return 0;
}


void print_curve(unsigned degree, coord_t* x, coord_t* y){
    unsigned length = 1 << (2*degree);
    printf("\n\n\n\n\nArray der Koordinaten:\n\n");
    for(unsigned i = 0; i < length; ++i) {
        printf("(%d,%d) ", x[i].val, y[i].val);
    }
    printf("\n");
}


void add_segments(unsigned segment_degree, coord_t* x, coord_t* y){
    unsigned segment_length = 1 << (2 * (segment_degree)), segment_coord = (1 << segment_degree) - 1;
    for(unsigned i = 0; i < segment_length; ++i) {
        //left upper segment
        x[segment_length + i].val = x[i].val;
        y[segment_length + i].val = y[i].val + segment_coord + 1;

        //right upper segment
        x[2*segment_length + i].val = x[i].val + segment_coord + 1;
        y[2*segment_length + i].val = y[i].val + segment_coord + 1;

        //left lower segment
        unsigned temp = x[i].val;
        x[i].val = y[i].val;
        y[i].val = temp;

        //right lower segment
        x[3*segment_length + i].val = 2*segment_coord + 1 - x[i].val;
        y[3*segment_length + i].val = segment_coord - y[i].val;
    }
}


void hilbert(unsigned degree, coord_t* x, coord_t* y) {
    //curve for degree = 1
    x[0].val = 0; y[0].val = 0; x[1].val = 0; y[1].val = 1; x[2].val = 1; y[2].val = 1; x[3].val = 1; y[3].val = 0;

    for(unsigned i = 2; i <= degree; i++){
        add_segments(i-1, x, y);
    }
}


void hilbert_V1(unsigned degree, coord_t* x, coord_t* y) {
    /*printf("Not implemented yet\n");
    degree +=1;
    x[0].val = 0;
    y[0].val = 0;*/
    
    v_assembly(degree, x, y);
}

void compare(unsigned degree, coord_t* x1, coord_t* y1, coord_t* x2, coord_t* y2){
    unsigned length = 1 << (2 * degree);
    
    for(int i = 0; i < length; i++){
        if(x1[i].val != x2[i].val){
            printf("x values were different. c: %d, assemb;y: %d", x1[i], x2[i]);
            return;
        }
        
        if(y1[i].val != y2[i].val){
            printf("y values were different. c: %d, assemb;y: %d", y1[i], y2[i]);
            return;
        }
    }
    
    printf("congrats! both arrays have the sasme values");
}

void write_svg(char* output_file_svg, int degree, coord_t* x, coord_t* y) {
    unsigned length = 1 << (2*degree);
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

    for (unsigned i = 1; i < length; ++i)
        fprintf(svg_fp, ",%d0 %d0", x[i].val, y[i].val);

    fprintf(svg_fp,"\" transform=\"scale(1 -1) translate(0 -%d0)\"/>\n</svg>\n",win_size);
    fclose(svg_fp);
}


void write_txt(char* output_file_txt, int degree, coord_t* x, coord_t *y) {
    unsigned length = 1 << (2*degree);

    char file_name[strlen(output_file_txt) + 5];
    FILE* txt_fp = fopen(strcat(strcpy(file_name, output_file_txt),".txt\0"),"w");

    if (txt_fp == NULL) {
        printf("File kann nicht angelegt werden\n");
        return;
    }
    for (unsigned i = 0; i < length; ++i)
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
