#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wav_io.h>

int rename_file_with_extension(char* filename, char* ext) {
    char* file_with_ext = malloc(strlen(filename) + MAX_EXTENSION_SIZE + 1);
    strcpy(file_with_ext, filename);
    strcat(file_with_ext, ext);
    rename(filename, file_with_ext);
    return 0;
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

long little_to_big_4_bytes(unsigned char buffer4[]) {
    return buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
}

long little_to_big_2_bytes(unsigned char buffer2[]) { return buffer2[0] | (buffer2[1] << 8); }

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
