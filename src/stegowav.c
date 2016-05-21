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

    return 0;
}
