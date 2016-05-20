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
    if (argc < 3){
        printf("Wrong parameters.\n");
        exit(1);
    }
    char *filename = argv[1];

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
    FILE *ptr_write = fopen("samples/dup.wav", "w");

    int mode = 0;
    if (strcmp("lsb1",argv[2]) == 0){
        mode = LSB1;
    } else {
        mode = LSB4;
    }

    // Obtain data of the file to hide
    FILE *file_to_hide = fopen("samples/file_to_hide.txt", "r");
    if (file_to_hide == NULL) {
      printf("Error opening file\n");
      exit(1);
    }
    char* data = file_to_char_array(file_to_hide);
    int data_size = strlen(data);

    // Write the data
    write_headers(&header, ptr_write);
    write_steg_sound_data(&header, ptr_write, data, strlen(data), mode);

    fclose(ptr);
    free(data);

    printf("Reading hidden info in samples/dup.wav...\n");

    ptr = fopen("samples/dup.wav", "r");
    char *hidden_data = (char *) calloc(data_size + 1, sizeof(char));

    read_headers(&header, ptr);
    read_steg_sound_data(&header, hidden_data, data_size, mode);

    fclose(ptr);

    ptr = fopen("samples/hidden_file.txt", "w");

    // Write obtained hidden_data
    int results = fputs(hidden_data, ptr);
    if (results == EOF) {
      printf("Failed to write\n");
      exit(1);
    }
    fclose(ptr);

    return 0;
}
