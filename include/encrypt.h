#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "arguments.h"

int crypto_encrypt(Encryption *params, char *plaintext, size_t plaintext_len,
                   char *ciphertext);

int crypto_decrypt(Encryption *params, char *ciphertext, size_t ciphertext_len,
                   char *decryptedtext);

#endif
