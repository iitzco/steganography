#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *open_file(char *file, char *mode) {
    FILE *ptr = fopen(file, mode);
    if (ptr == NULL) {
        perror(file);
        exit(1);
    }
    return ptr;
}

unsigned long get_file_size(FILE *file) {
    unsigned long lSize;
    fseek(file, 0L, SEEK_END);
    lSize = ftell(file);
    rewind(file);
    return lSize;
}

long little_to_big_4_bytes(unsigned char buffer4[]) {
    return buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) |
           (buffer4[3] << 24);
}

long little_to_big_2_bytes(unsigned char buffer2[]) {
    return buffer2[0] | (buffer2[1] << 8);
}

char *file_to_char_array(FILE *file) {
    long lSize;
    char *buffer;

    fseek(file, 0L, SEEK_END);
    lSize = ftell(file);
    rewind(file);

    // Allocate memory for entire content
    buffer = calloc(1, lSize + 1);
    if (!buffer) {
        printf("Error allocating\n");
        fclose(file);
        exit(1);
    }

    // Copy the file into the buffer
    if (1 != fread(buffer, lSize, 1, file)) {
        fclose(file);
        free(buffer);
        printf("Error reading file\n");
        exit(1);
    }

    fclose(file);
    return buffer;
}

void dec_to_num_representation(unsigned long value, unsigned char vec[]) {
    unsigned char i = 0;
    unsigned long BASE = 256;
    while (value > BASE) {
        vec[i] = (unsigned char)(value % BASE);
        i++;
        value /= BASE;
    }
    vec[i] = value;
}

unsigned long num_representation_to_dec(unsigned char vec[], int vec_size) {
    unsigned char i = 0;
    unsigned long count = 0;
    unsigned long BASE = 256;
    while (i < vec_size) {
        count += (unsigned int)(vec[i] * pow(BASE, i));
        i++;
    }
    return count;
}
