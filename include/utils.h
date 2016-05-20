#ifndef UTILS_H
#define UTILS_H

char* seconds_to_time(float seconds);

long little_to_big_4_bytes(unsigned char buffer4[]);

long little_to_big_2_bytes(unsigned char buffer2[]);

char* file_to_char_array(FILE *file);

#endif
