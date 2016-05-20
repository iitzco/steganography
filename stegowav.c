#include "arguments.h"
#include "printer.h"
#include "wav_io.h"
#include "lsb.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    parseArguments(argc,argv);
    HEADER header;

    //Initialize in 0
    memset(&header, 0, sizeof(header));

    FILE *ptr;
    if (argc < 4){
        printf("Wrong parameters.\n");
        exit(1);
    }
    char *filename = get_file_path(argv[1]);
    if (filename==NULL) {
        exit(1);
    }

    // open file

    printf("Opening file...\n");
    ptr = fopen(filename, "r");
    if (ptr == NULL) {
       printf("Error opening file\n");
       exit(1);
    }

    printf("Reading file...\n");
    read_headers(&header,ptr);


    printf("Writing hidden info in samples/dup.wav...\n");
    filename = get_file_path("samples/dup.wav");
    if (filename==NULL) {
        exit(1);
    }
    FILE *ptr_write = fopen(filename, "w");

    int mode = 0;
    // Write msg on wav
    if (strcmp("lsb1",argv[2]) == 0){
        mode = LSB1;
    } else {
        mode = LSB4;
    }
    char * msg = argv[3];

    write_headers(&header, ptr_write);
    write_steg_sound_data(&header, ptr_write, msg, strlen(msg), mode);

    fclose(ptr);
    free(filename);


    printf("Reading hidden info in samples/dup.wav...\n");
    filename = get_file_path("samples/dup.wav");

    if (filename==NULL) {
        exit(1);
    }

    ptr = fopen(filename, "r");
    char *hidden_msg = (char *) calloc(strlen(msg)+1, sizeof(char));

    read_headers(&header, ptr);
    read_steg_sound_data(&header, hidden_msg, strlen(msg), mode);
    printf("Hidden info:\n%s\n", hidden_msg);

    return 0;

}
