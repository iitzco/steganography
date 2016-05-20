#include "encrypt.h"
#include <openssl/evp.h>

char* encrypt(char* data, encrypt_cipher cipher, encrypt_mode mode, char* password) {
	unsigned char* out = calloc(1, 256);
	int inl, outl, templ;
    unsigned char* key = malloc(256 + EVP_MAX_IV_LENGTH);
    unsigned char* iv = malloc(EVP_MAX_IV_LENGTH);
	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);

	EVP_BytesToKey(EVP_aes_128_cbc(), EVP_md5(), NULL, password, strlen(password), 5, key, iv);

	EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv); // Todo: Define cipher and mode
	inl = strlen(data);
	EVP_EncryptUpdate(&ctx, out, &outl, data, inl);
	EVP_EncryptFinal(&ctx, out + outl, &templ);

	EVP_CIPHER_CTX_cleanup(&ctx);

	free(key);
	free(iv);

	return out;
}

char* decrypt(char* data, encrypt_cipher cipher, encrypt_mode mode, char* password) {

}