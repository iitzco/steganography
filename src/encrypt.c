#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/obj_mac.h>
#include <string.h>
#include "arguments.h"

/* A 128 bit IV */
unsigned char *iv = (unsigned char *)"0156789012678901";

void crypto_setup(void) {
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
}

void crypto_teardown(void) {
    EVP_cleanup();
    ERR_free_strings();
}

void crypto_handle_error(void) {
    ERR_print_errors_fp(stderr);
    exit(1);
}

int crypto_get_cipher_nid(CipherAlgorithm algo, CipherMode mode) {
    static int nids[ALGO_MAX][MODE_MAX] = {
        {0, 0, 0, 0, 0},
        {0, NID_aes_128_ecb, NID_aes_128_cfb8, NID_aes_128_ofb128, NID_aes_128_cbc},
        {0, NID_aes_192_ecb, NID_aes_192_cfb8, NID_aes_192_ofb128, NID_aes_192_cbc},
        {0, NID_aes_256_ecb, NID_aes_256_cfb8, NID_aes_256_ofb128, NID_aes_256_cbc},
        {0, NID_des_ecb, NID_des_cfb8, NID_des_ofb64, NID_des_cbc}};

    int nid = nids[algo][mode];
    if (nid == 0) {
        fprintf(stderr, "fatal error: invalid cipher algorithm\n");
        exit(1);
    }
    return nid;
}

void crypto_get_key(char *password, unsigned char *key) {
    /* if (PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), NULL, 0, 1000, 32, key) != 1) */
    /*     crypto_handle_error(); */
    MD5((const unsigned char *)password, strlen(password), key);
}

int crypto_encrypt(Encryption *params, char *plaintext, size_t plaintext_len, char *ciphertext) {
    int len, ciphertext_len;

    unsigned char key[64];
    crypto_get_key(params->password, key);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) crypto_handle_error();

    int nid = crypto_get_cipher_nid(params->algorithm, params->mode);
    const EVP_CIPHER *cipher = EVP_get_cipherbynid(nid);

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) != 1) crypto_handle_error();

    if (EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &len, (unsigned char *)plaintext,
                          plaintext_len) != 1)
        crypto_handle_error();

    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, (unsigned char *)ciphertext + len, &len) != 1)
        crypto_handle_error();

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int crypto_decrypt(Encryption *params, char *ciphertext, size_t ciphertext_len,
                   char *decryptedtext) {
    int len, decryptedtext_len;

    unsigned char key[64];
    crypto_get_key(params->password, key);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) crypto_handle_error();

    int nid = crypto_get_cipher_nid(params->algorithm, params->mode);
    const EVP_CIPHER *cipher = EVP_get_cipherbynid(nid);

    if (EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv) != 1) crypto_handle_error();

    if (EVP_DecryptUpdate(ctx, (unsigned char *)decryptedtext, &len, (unsigned char *)ciphertext,
                          ciphertext_len) != 1)
        crypto_handle_error();

    decryptedtext_len = len;

    if (EVP_DecryptFinal_ex(ctx, (unsigned char *)decryptedtext + len, &len) != 1)
        crypto_handle_error();

    decryptedtext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    return decryptedtext_len;
}

/*
int main(void) {
    char* plaintext = "The quick brown fox jumps over the lazy dog";
    Encryption params = { .algorithm = AES256, .mode = CBC, .password = "123456"
};

    char ciphertext[256];
    char decryptedtext[256];

    int decryptedtext_len, ciphertext_len;
    int plaintext_len = strlen(plaintext);

    crypto_setup();

    ciphertext_len = crypto_encrypt(&params, plaintext, plaintext_len,
ciphertext);
    decryptedtext_len = crypto_decrypt(&params, ciphertext, ciphertext_len,
decryptedtext);

    printf("%s\n", decryptedtext);

    crypto_teardown();

    return 0;
}
*/
