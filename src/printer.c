#include "printer.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "wav_io.h"

void print_all_headers(HEADER *header) {
    printf("RIFF HEADER\n");
    printf("%.4s\n", header->riff);

    printf("OVERALL SIZE HEADER\n");
    printf("%d\n", header->overall_size);

    printf("FORMAT HEADER\n");
    printf("%.4s\n", header->wave);

    printf("FMT HEADER\n");
    printf("%s\n", header->fmt_chunk_marker);

    printf("FORMAT CATEGORY (16 for PCM)\n");
    printf("%d\n", header->length_of_fmt);

    char format_name[10] = "";
    if (header->format_type == 1)
        strcpy(format_name, "PCM");
    else if (header->format_type == 6)
        strcpy(format_name, "A-law");
    else if (header->format_type == 7)
        strcpy(format_name, "Mu-law");

    printf("AUDIO FORMAT CATEGORY (1 for PCM)\n");
    printf("%d (%s)\n", header->format_type, format_name);

    printf("AUDIO CHANNEL\n");
    printf("%d \n", header->channels);

    printf("AUDIO SAMPLE RATE\n");
    printf("%d \n", header->sample_rate);

    printf("AUDIO BYTE RATE\n");
    printf("%d \n", header->byterate);

    printf("BLOCK ALIGNMENT\n");
    printf("%d \n", header->block_align);

    printf("BYTES PER SAMPLE\n");
    printf("%d \n", header->bits_per_sample);

    printf("DATA HEADER\n");
    printf("%s\n", header->data_chunk_header);

    printf("DATA SIZE\n");
    printf("%d \n", header->data_size);
}
