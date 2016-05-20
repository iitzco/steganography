#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "lsb.h"
#include "printer.h"
#include "utils.h"
#include "wav_io.h"

int main(int argc, char **argv) {
    ARGUMENTS arguments;
    HEADER header;

    args_parse(argc, argv, &arguments);

    // Initialize in 0
    memset(&header, 0, sizeof(header));

    FILE *ptr;
    char *filename = arguments.p_wavefile;

    printf("Opening file %s...\n", arguments.p_wavefile);

    ptr = fopen(filename, "r");
    if (ptr == NULL) {
        perror(filename);
        exit(1);
    }

    printf("Reading file...\n");
    wav_header_read(&header, ptr);

    printf("Writing hidden info in %s...\n", arguments.out_file);
    filename = arguments.out_file;
    FILE *ptr_write = fopen(filename, "w");
    if (ptr_write == NULL) {
        perror(filename);
        exit(1);
    }

    filename = arguments.in_file;
    FILE *ptr_in_data = fopen(filename, "r");
    if (ptr_in_data == NULL) {
        perror(filename);
        exit(1);
    }

    char *data = file_to_char_array(ptr_in_data);
    int data_size = strlen(data);

    int mode = 0;
    if (arguments.steg == LSB1) {
        mode = LSB1;
    } else if (arguments.steg == LSB4) {
        mode = LSB4;
    }

    wav_header_write(&header, ptr_write);

    wav_stego_encode(&header, ptr_write, data, data_size, mode);

    fclose(ptr);
    fclose(ptr_in_data);
    fclose(ptr_write);

    printf("Reading hidden info from %s...\n", arguments.out_file);
    filename = arguments.out_file;

    ptr = fopen(filename, "r");
    char *hidden_msg = (char *)calloc(data_size + 1, sizeof(char));

    wav_header_read(&header, ptr);

    wav_stego_decode(&header, hidden_msg, data_size, mode);

    printf("\nHidden info:\n%s\n", hidden_msg);

    return 0;
}
