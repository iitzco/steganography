#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "arguments.h"

typedef A encrypt_cipher;
typedef M encrypt_mode;

char* encrypt(char* data, encrypt_cipher cipher, encrypt_mode mode, char* password);

char* decrypt(char* data, encrypt_cipher cipher, encrypt_mode mode, char* password);

#endif
