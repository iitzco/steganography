#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "lsb.h"

typedef enum { MODE_NONE, EMBED, EXTRACT } ProgramMode;
typedef enum { A_NONE, AES128, AES192, AES256, DES } CipherAlgorithm;
typedef enum { M_NONE, ECB, CFB, OFB, CBC } CipherMode;

#define ALGO_MAX 5
#define MODE_MAX 5

typedef struct {
    CipherAlgorithm algorithm;
    CipherMode mode;
    char* password;
} Encryption;

typedef struct {
    ProgramMode mode;
    char* in_file;
    char* p_wavefile;
    char* out_file;
    StegMode steg;
    Encryption encryption;
} Arguments;

void args_print(Arguments* arguments);

void args_parse(int argc, char** argv, Arguments* arguments);

#endif
