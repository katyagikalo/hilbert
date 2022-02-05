#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main.h"
#include "print.h"

void print_parameter(parameter parameter_args){
    if (parameter_args.test_file
    ){
        printf( "\nVersion                  : Create files for all Versions\n");
    }
    else if (parameter_args.test_time) {
        printf( "\nVersion                  : Benchmark all Versions from 1 to %d\n", parameter_args.degree);
    }
    else if (parameter_args.version == -1) {
        printf(   "\nVersion                  : Default\n");
    }
    else {
        printf(   "\nVersion                  : V%d\n",parameter_args.version);
    }
    if (parameter_args.test_file){
        printf("Grad der Hilbertkurve    : %d\n",parameter_args.degree);
    }
    else if (parameter_args.test_time){
        printf("Grad der Hilbertkurve    : 1 to %d\n",parameter_args.degree);
    }
    else{
        printf("Grad der Hilbertkurve    : %d\n",parameter_args.degree);
    }
    
    if (parameter_args.test_time) {
        printf("Zeitmessung              : ja\n\n\n");
    }
    else if (parameter_args.test_file) {
        printf("Dateien werden erstellt\n");
    }
    else {
        printf("Zeitmessung              : %s\n",
            (parameter_args.messure_time ? "ja" : "nein"));
    }
    
    if (!parameter_args.test_file && !parameter_args.test_time)
        printf("Funktionsaufrufe         : %d\n"
               "AusgabeArray auf Konsole : %s\n"
               "SVG_Ausgabedatei         : %s\n"
               "TXT_Ausgabedatei         : %s\n\n\n",
               parameter_args.count_call, (parameter_args.print_console ? "ja" : "nein"),
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


double print_time(struct timespec start, struct timespec end) {
     double elapsed = (end.tv_sec - start.tv_sec);
     elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
     printf("%.10f Sekunden\n", elapsed);
     return elapsed;
}

void print_result(unsigned degree, double result[][7]) {
    for (unsigned i=0; i<degree; i++) {

        unsigned temp = 0;
        double val = result[i][0];

        for (unsigned j=1; j<=6; j++) {
            if (result[i][j] < val) {
                val = result[i][j];
                temp = j;
            }
        }
        printf("The fastest time for n=%d was Version %d\n", i+1, temp);
    }
}

void create_folder(char path[]){
    struct stat st_0 = {0};
    
    if (stat(path, &st_0) == -1)
        mkdir(path, 0700);
    
    printf("Ordner %s erstellt\n", path);
}


void write_svg(char *path, char *output_file_svg, int degree, coord_t* x, coord_t* y) {

    FILE *svg_fp;
    
    if (path == NULL) {
        char path_file[strlen(output_file_svg) + 5];
        strcpy(path_file, output_file_svg);
        
        svg_fp = fopen(strcat(path_file, ".svg\0"),"w");
    }

    else {
        char file_name[strlen(output_file_svg)];
        char path_file[strlen(output_file_svg) + strlen(path) + 6];
        strcpy(file_name, output_file_svg);
        strcpy(path_file, path);
        strcat(path_file, "/");
        strcat(path_file, file_name);
        
        svg_fp = fopen(strcat(path_file,".svg\0"),"w");
    }
    
    if (svg_fp == NULL) {
        printf("File kann nicht angelegt werden\n");
        return;
    }

    unsigned long long length = (unsigned long long)1 << (2*degree);
    unsigned win_size = 1 << degree;
    
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
    printf("Datei %s.svg erstellt\n", output_file_svg);
}

void write_txt(char *path, char *output_file_txt, int degree, coord_t* x, coord_t* y) {

    FILE *txt_fp;
    
    if (path == NULL) {
        char path_file[strlen(output_file_txt) + 5];
        strcpy(path_file, output_file_txt);
        
        txt_fp = fopen(strcat(path_file, ".txt\0"),"w");
    }

    else {
        char file_name[strlen(output_file_txt)];
        char path_file[strlen(output_file_txt) + strlen(path) + 6];
        strcpy(file_name, output_file_txt);
        strcpy(path_file, path);
        strcat(path_file, "/");
        strcat(path_file, file_name);
        
        txt_fp = fopen(strcat(path_file,".txt\0"),"w");
    }
    
    if (txt_fp == NULL) {
        printf("File kann nicht angelegt werden\n");
        return;
    }
    
    unsigned long long length = (unsigned long long)1 << (2*degree);
    
    for (unsigned long long i = 0; i < length; ++i)
        fprintf(txt_fp, "(%d,%d)", x[i].val, y[i].val);
    fclose(txt_fp);
    printf("Datei %s.txt erstellt\n", output_file_txt);
}


void help_message() {
    printf("\nDefaultwerte sind: -V0 -n1\n\n"
           "./main [-V<int>] [-B<int>] [-n<int>] [-o<file>] [-h] [--help]\n\n"
           "Options:\n"
           "  -V<int>        --Version                                                         <int>   -> V0 bis V5\n"
           "  -B<int>[opt]   --Laufzeitmessung                                                 <int>   -> Anzahl Aufrufe\n"
           "  -n<int>        --Grad der Hilbertkurve                                           <int>   -> Grad der Hilbertkurve\n"
           "  -p             --Koordinatenausgabe auf Konsole\n"
           "  -t<int>        --Anzahl an Threads zur Berechnung                                <int>   -> Thread Anzahl\n"
           "  -o<file>       --SVG Ausgabedatei                                                <file>  -> Name der SVG Ausgabedatei\n"
           "  -u<file>       --txt Ausgabedatei                                                <file>  -> Name der txt Ausgabedatei\n"
           "  --test_file    --Erstellt von jeder Version eine txt und eine svg Datei"
           "  --test_time    --Testet alle Versionen von 1 bis n"
           "  -h             --Hilfe\n"
           "  --help         --Hilfe\n\n\n"
           "Beispielaufrufe:   ./main -h\n"
           "                   ./main --help\n"
           "                   ./main --test_time\n"
           "                   ./main --test_all\n"
           "                   ./main --test_time -n15 -t8\n"
           "                   ./main --test_all -n13 -t32\n"
           "                   ./main\n"
           "                   ./main -B -n4\n"
           "                   ./main -o file_name -n5\n"
           "                   ./main -V1 -B5 -n3 -o file_name\n"
           "                   ./main -V0 -B12 -n4 -o my_hilbert_svg -u my_hilbert_txt -t16\n\n\n"
           "Wird eine Option mit Optionsargument doppelt aufgerufen, so wird das letzte Optionsargument verwendet.\n\n\n\n");
}
