// WAVE file header format
// Source: http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
#ifndef WAVPARSER_H
#define WAVPARSER_H

#include <stdio.h>
#include "lsb.h"

#define HEADER_SIZE 44
#define BLOCK_SIZE 1024

typedef struct {
    unsigned char native_header[HEADER_SIZE];  // Full native header
    unsigned char riff[4];                     // RIFF string
    unsigned int overall_size;                 // overall size of file in bytes
    unsigned char wave[4];                     // WAVE string
    unsigned char fmt_chunk_marker[4];  // fmt string with trailing null char
    unsigned int length_of_fmt;         // length of the format data
    unsigned int format_type;  // format type. 1-PCM, 3- IEEE float, 6 - 8bit A
                               // law, 7 - 8bit mu law
    unsigned int channels;     // no.of channels
    unsigned int sample_rate;  // sampling rate (blocks per second)
    unsigned int byterate;     // SampleRate * NumChannels * BitsPerSample/8
    unsigned int block_align;  // NumChannels * BitsPerSample/8
    unsigned int bits_per_sample;  // bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header[4];  // DATA string or FLLR string
    unsigned int data_size;  // NumSamples * NumChannels * BitsPerSample/8 -
                             // size of the next chunk that will be read
    FILE *ptr;               // sound data FILE pointer
} HEADER;

int wav_header_read(HEADER *header, FILE *ptr);

int wav_header_write(HEADER *header, FILE *ptr);

int wav_stego_encode(HEADER *header, FILE *ptr, FILE *msg, Steg mode);

int wav_stego_decode(HEADER *header, FILE *output, Steg mode);

#endif
