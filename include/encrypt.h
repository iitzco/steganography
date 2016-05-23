#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "arguments.h"

char* encrypt(char* data, CipherAlgorithm algorithm, CipherMode mode,
              char* password);

char* decrypt(char* data, CipherAlgorithm algorithm, CipherMode mode,
              char* password);

#endif
