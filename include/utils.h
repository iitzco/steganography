#ifndef UTILS_H
#define UTILS_H

char* get_mode_str(StegMode mode);

char* get_filename_ext(const char* filename);

int rename_file_with_extension(char* filename, char* ext);

FILE* open_file(char* file, char* mode);

unsigned long get_file_size(FILE* file);

void dec_to_num_representation(unsigned long value, unsigned char vec[], int vec_size);

unsigned long get_max_capacity(StegMode mode, FILE* carrier);

unsigned long num_representation_to_dec(unsigned char vec[], int vec_size);

long little_to_big_4_bytes(unsigned char buffer4[]);

long little_to_big_2_bytes(unsigned char buffer2[]);

#endif
