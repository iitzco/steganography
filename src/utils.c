#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "wav_io.h"

int rename_file_with_extension(char* filename, char* ext) {
    char* file_with_ext = malloc(strlen(filename) + MAX_EXTENSION_SIZE + 1);
    strcpy(file_with_ext, filename);
    strcat(file_with_ext, ext);
    rename(filename, file_with_ext);
    return 0;
}

long little_to_big_4_bytes(unsigned char buffer4[]) {
    return buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
}

long little_to_big_2_bytes(unsigned char buffer2[]) { return buffer2[0] | (buffer2[1] << 8); }

char* get_mode_str(StegMode mode) {
    if (mode == LSB1) return "LSB1";
    if (mode == LSB4) return "LSB4";
    return "LSBE";
}

char* get_filename_ext(const char* filename) {
    char* dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot;
}

FILE* open_file(char* file, char* mode) {
    FILE* ptr = fopen(file, mode);
    if (ptr == NULL) {
        perror(file);
        exit(1);
    }
    return ptr;
}

unsigned long get_file_size(FILE* file) {
    unsigned long lSize;
    fseek(file, 0L, SEEK_END);
    lSize = ftell(file);
    rewind(file);
    return lSize;
}

void dec_to_num_representation(unsigned long value, unsigned char vec[], int vec_size) {
    unsigned char i = 0;
    unsigned long BASE = 256;
    while (value > BASE) {
        vec[vec_size - i - 1] = (unsigned char)(value % BASE);
        i++;
        value /= BASE;
    }
    vec[vec_size - i - 1] = value;
}

unsigned long num_representation_to_dec(unsigned char vec[], int vec_size) {
    unsigned char i = 0;
    unsigned long count = 0;
    unsigned long BASE = 256;
    while (i < vec_size) {
        count += (unsigned int)(vec[i] * pow(BASE, vec_size - i - 1));
        i++;
    }
    return count;
}

unsigned long get_max_capacity(StegMode mode, FILE* carrier) {
    WavHeader header;
    wav_header_read(&header, carrier);

    char sample_size = header.bits_per_sample / 8;
    char block_byte_size = 0;

    if (mode != LSBE) {
        if (mode == LSB1) {
            block_byte_size = sample_size * 8;
        } else if (mode == LSB4) {
            block_byte_size = sample_size * 2;
        }
        return ((get_file_size(carrier) - HEADER_SIZE) / block_byte_size) - 4 - MAX_EXTENSION_SIZE;
    } else {
        unsigned char byte[1];
        unsigned long count = 0;

        while (fread(byte, 1, 1, header.ptr) == 1) {
            if (byte[0] == 254 || byte[0] == 255) count++;
        }
        unsigned long bytes = count / 8;
        if (bytes < 4 + MAX_EXTENSION_SIZE) return 0;
        return bytes - 4 - MAX_EXTENSION_SIZE;
    }
}
