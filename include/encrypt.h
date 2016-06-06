#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <openssl/evp.h>
#include "arguments.h"

typedef EVP_CIPHER_CTX CipherContext;

void crypto_setup(void);

void crypto_teardown(void);

CipherContext *crypto_encrypt_init(Encryption *params);

int crypto_encrypt_update(CipherContext *ctx, char *plaintext, int plaintext_len, char *ciphertext);

int crypto_encrypt_final(CipherContext *ctx, char *ciphertext);

CipherContext *crypto_decrypt_init(Encryption *params);

int crypto_decrypt_update(CipherContext *ctx, char *ciphertext, int ciphertext_len, char *decryptedtext);

int crypto_decrypt_final(CipherContext *ctx, char *decryptedtext);

#endif
