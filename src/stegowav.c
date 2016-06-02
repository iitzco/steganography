#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "encrypt.h"
#include "lsb.h"
#include "printer.h"
#include "utils.h"
#include "wav_io.h"

int embed_data(WavHeader* header, Arguments* arguments) {
    FILE* ptr = open_file(arguments->p_wavefile, "r");
    FILE* ptr_write = open_file(arguments->out_file, "w");
    FILE* ptr_in_data = open_file(arguments->in_file, "r");

    wav_header_read(header, ptr);
    wav_header_write(header, ptr_write);

    char* ext = get_filename_ext(arguments->in_file);
    wav_stego_encode(header, ptr_write, ptr_in_data, arguments->steg, ext);
    printf("Encoding %s\n", ext);

    return 0;
}

int extract_data(WavHeader* header, Arguments* arguments) {
    FILE* ptr = open_file(arguments->p_wavefile, "r");
    FILE* ptr_write = open_file(arguments->out_file, "w");

    wav_header_read(header, ptr);

    char* ext = (char*)calloc(MAX_EXTENSION_SIZE, 1);
    int ret = wav_stego_decode(header, ptr_write, arguments->steg, ext);
    if (ret == -1) {
        printf("Error while decoding file.\n");
        return -1;
    }
    rename_file_with_extension(arguments->out_file, ext);
    return 0;
}

int main(int argc, char** argv) {
    Arguments arguments;
    args_parse(argc, argv, &arguments);

    WavHeader header;
    memset(&header, 0, sizeof(header));

    if (arguments.mode == EMBED) {
        embed_data(&header, &arguments);
    } else if (arguments.mode == EXTRACT) {
        extract_data(&header, &arguments);
    } else {
        usage();
    }

    return 0;
}
