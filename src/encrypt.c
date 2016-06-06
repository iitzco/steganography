#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/obj_mac.h>
#include <string.h>
#include "arguments.h"
#include "encrypt.h"

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

void crypto_get_key(const EVP_CIPHER *cipher, char *password, unsigned char *key, unsigned char *iv) {
    /* if (PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), NULL, 0, 1000, 32, key) != 1) */
    /*     crypto_handle_error(); */

    if (EVP_BytesToKey(cipher, EVP_md5(), NULL, (unsigned char *)password, strlen(password),1, key, iv) == 0)
        crypto_handle_error();
}

CipherContext *crypto_encrypt_init(Encryption *params) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) crypto_handle_error();

    int nid = crypto_get_cipher_nid(params->algorithm, params->mode);
    const EVP_CIPHER *cipher = EVP_get_cipherbynid(nid);

    unsigned char key[64], iv[64];
    crypto_get_key(cipher, params->password, key, iv);

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
        crypto_handle_error();

    return ctx;
}

int crypto_encrypt_update(CipherContext *ctx, char *plaintext, int plaintext_len, char *ciphertext) {

    int ciphertext_len;

    if (EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &ciphertext_len,
                          (unsigned char *)plaintext,  plaintext_len) != 1)
        crypto_handle_error();

    return ciphertext_len;
}

int crypto_encrypt_final(CipherContext *ctx, char *ciphertext) {

    int ciphertext_len;

    if (EVP_EncryptFinal_ex(ctx, (unsigned char *)ciphertext, &ciphertext_len) != 1)
        crypto_handle_error();

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

CipherContext *crypto_decrypt_init(Encryption *params) {

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) crypto_handle_error();

    int nid = crypto_get_cipher_nid(params->algorithm, params->mode);
    const EVP_CIPHER *cipher = EVP_get_cipherbynid(nid);

    unsigned char key[64], iv[64];
    crypto_get_key(cipher, params->password, key, iv);

    if (EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv) != 1)
        crypto_handle_error();

    return ctx;
}

int crypto_decrypt_update(CipherContext *ctx, char *ciphertext, int ciphertext_len, char *decryptedtext) {

    int decryptedtext_len;

    if (EVP_DecryptUpdate(ctx, (unsigned char *)decryptedtext, &decryptedtext_len,
                          (unsigned char *)ciphertext, ciphertext_len) != 1)
        crypto_handle_error();

    return decryptedtext_len;
}

int crypto_decrypt_final(CipherContext *ctx, char *decryptedtext) {

    int decryptedtext_len;

    if (EVP_DecryptFinal_ex(ctx, (unsigned char *)decryptedtext, &decryptedtext_len) != 1)
        crypto_handle_error();

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

    int len;
    int decryptedtext_len = 0, ciphertext_len = 0;
    int plaintext_len = strlen(plaintext);

    crypto_setup();

    // Encryption
    CipherContext *ctx = crypto_encrypt_init(&params);

    len = crypto_encrypt_update(ctx, plaintext, plaintext_len, ciphertext);
    ciphertext_len += len;
    
    len = crypto_encrypt_final(ctx, ciphertext + ciphertext_len);
    ciphertext_len += len;

    BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);

    // Decryption
    ctx = crypto_decrypt_init(&params);

    len = crypto_decrypt_update(ctx, ciphertext, ciphertext_len, decryptedtext);
    decryptedtext_len += len;

    len = crypto_decrypt_final(ctx, decryptedtext + decryptedtext_len);
    decryptedtext_len += len;

    ctx = NULL;

    printf("%s\n", decryptedtext);

    crypto_teardown();

    return 0;
}
*/
