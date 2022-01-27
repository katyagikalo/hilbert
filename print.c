#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "print.h"

void print_parameter(parameter parameter_args){
    printf("Version                  : V%d\n"
           "Grad der Hilbertkurve    : %d\n"
           "Zeitmessung              : %s\n"
           "Funktionsaufrufe         : %d\n"
           "AusgabeArray auf Konsole : %s\n"
           "SVG_Ausgabedatei         : %s\n"
           "TXT_Ausgabedatei         : %s\n",
           parameter_args.version, parameter_args.degree, (parameter_args.messure_time ? "ja" : "nein"), parameter_args.count_call,
           (parameter_args.print_console ? "ja" : "nein"),
           (parameter_args.write_svg_file ? parameter_args.output_file_svg : "--nicht gewaehlt--"),
           (parameter_args.write_txt_file ? parameter_args.output_file_txt : "--nicht gewaehlt--"));
}

void print_curve(unsigned degree, coord_t* x, coord_t* y){
    unsigned long long length = (unsigned long long)1 << (2*degree);
    printf("\n\n\n\n\nArray der Koordinaten:\n\n");
    for(unsigned long long i = 0; i < length; ++i) {
        printf("(%d,%d) ", x[i].val, y[i].val);
    }
    printf("\n");
}

void print_time(struct timespec *start, struct timespec *end) {
     double elapsed = (end->tv_sec - start->tv_sec);
     elapsed += (end->tv_nsec - start->tv_nsec) / 1000000000.0;
     printf("Zeitmessung ergibt: %f Sekunden\n", elapsed);
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
