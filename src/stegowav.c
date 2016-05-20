#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "lsb.h"
#include "printer.h"
#include "utils.h"
#include "wav_io.h"

int embed_data(HEADER *header, ARGUMENTS *arguments) {
    FILE *ptr = open_file(arguments->p_wavefile, "r");
    FILE *ptr_write = open_file(arguments->out_file, "w");
    FILE *ptr_in_data = open_file(arguments->in_file, "r");

    wav_header_read(header, ptr);
    wav_header_write(header, ptr_write);

    wav_stego_encode(header, ptr_write, ptr_in_data, arguments->steg);

    return 0;
}

int extract_data(HEADER *header, ARGUMENTS *arguments) {
    FILE *ptr = open_file(arguments->p_wavefile, "r");
    FILE *ptr_write = open_file(arguments->out_file, "w");

    wav_header_read(header, ptr);

    wav_stego_decode(header, ptr_write, arguments->steg);

    return 0;
}

int main(int argc, char **argv) {
    ARGUMENTS arguments;
    args_parse(argc, argv, &arguments);

    HEADER header;
    memset(&header, 0, sizeof(header));

    if (arguments.mode == EMBED) {
        printf("EMBED\n");
        embed_data(&header, &arguments);
    } else if (arguments.mode == EXTRACT) {
        printf("EXTRACT\n");
        extract_data(&header, &arguments);
    }

    /* printf("Reading hidden info from %s...\n", arguments.out_file); */

    /* FILE *ptr = fopen(arguments.out_file, "r"); */
    /* if (ptr == NULL) { */
    /*     perror(arguments.p_wavefile); */
    /*     exit(1); */
    /* } */

    /* char *hidden_msg = (char *)calloc(data_size + 1, sizeof(char)); */

    /* wav_header_read(&header, ptr); */

    /* wav_stego_decode(&header, hidden_msg, data_size, mode); */

    /* printf("\nHidden info:\n%s\n", hidden_msg); */

    return 0;
}
