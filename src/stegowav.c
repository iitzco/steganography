#include "printer.h"
#include "wav_io.h"
#include "lsb.h"
#include "arguments.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    ARGUMENTS arguments;
    HEADER header;

    parseArguments(argc, argv, &arguments);

    //Initialize in 0
    memset(&header, 0, sizeof(header));

    FILE *ptr;
    char *filename = arguments.p_wavefile;

    printf("Opening file %s...\n", arguments.p_wavefile);
    ptr = fopen(filename, "r");
    if (ptr == NULL) {
       printf("Error opening file\n");
       exit(1);
    }

    printf("Reading file...\n");
    read_headers(&header,ptr);

    printf("Writing hidden info in %s...\n", arguments.out_file);
    filename = arguments.out_file;
    FILE *ptr_write = fopen(filename, "w");

    filename = arguments.in_file;
    FILE *ptr_in_data = fopen(filename, "r");

    char* data = file_to_char_array(ptr_in_data);
    int data_size = strlen(data);

    int mode = 0;
    if (arguments.steg == LSB1){
                mode = LSB1;
    } else if (arguments.steg == LSB4){
                mode = LSB4;
    }

    write_headers(&header, ptr_write);

    write_steg_sound_data(&header, ptr_write, data, data_size, mode);

    fclose(ptr);
    fclose(ptr_in_data);
    fclose(ptr_write);

    printf("Reading hidden info from %s...\n", arguments.out_file);
    filename = arguments.out_file;

    ptr = fopen(filename, "r");
    char *hidden_msg = (char *) calloc(data_size+1, sizeof(char));

    read_headers(&header, ptr);

    read_steg_sound_data(&header, hidden_msg, data_size, mode);

    printf("\nHidden info:\n%s\n", hidden_msg);

    return 0;

}
