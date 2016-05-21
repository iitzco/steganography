#ifndef UTILS_H
#define UTILS_H

FILE* open_file(char* file, char* mode);

unsigned long get_file_size(FILE* file);

long little_to_big_4_bytes(unsigned char buffer4[]);

long little_to_big_2_bytes(unsigned char buffer2[]);

char* file_to_char_array(FILE* file);

void dec_to_num_representation(unsigned long value, unsigned char vec[]);

unsigned long num_representation_to_dec(unsigned char vec[], int vec_size);

#endif
