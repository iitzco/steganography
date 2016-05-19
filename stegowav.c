#include "printer.h"
#include "wav_io.h"
#include "lsb.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    HEADER header;

    //Initialize in 0
    memset(&header, 0, sizeof(header));

    FILE *ptr;
    if (argc < 3){
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
    free(filename);

    printf("Reading hidden info in samples/dup.wav...\n");
    filename = get_file_path("samples/dup.wav");

    if (filename==NULL) {
        exit(1);
    }

    ptr = fopen(filename, "r");
    char *hidden_data = (char *) calloc(data_size + 1, sizeof(char));

    read_headers(&header, ptr);
    read_steg_sound_data(&header, hidden_data, data_size, mode);

    fclose(ptr);
    free(filename);

    filename = get_file_path("samples/hidden_file.txt");
    if (filename==NULL) {
        exit(1);
    }
    ptr = fopen(filename, "w");

    // Write obtained hidden_data
    int results = fputs(hidden_data, ptr);
    if (results == EOF) {
      printf("Failed to write\n");
      exit(1);
    }
    free(filename);
    fclose(ptr);

    return 0;
}
