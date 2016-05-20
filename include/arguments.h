#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "lsb.h"

typedef enum t_mode { MODE_NONE, EMBED, EXTRACT } Mode;
typedef enum t_a { A_NONE, AES128, AES192, AES256, DES } A;
typedef enum t_m { M_NONE, ECB, CFB, OFB, CBC } M;

typedef struct {
    Mode mode;
    char* in_file;
    char* p_wavefile;
    char* out_file;
    Steg steg;
    A a;
    M m;
    char* pass;
} ARGUMENTS;

void print_arguments(ARGUMENTS* arguments);

void parse_arguments(int argc, char** argv, ARGUMENTS* arguments);

#endif
