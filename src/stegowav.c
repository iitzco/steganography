#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "encrypt.h"
#include "lsb.h"
#include "printer.h"
#include "utils.h"
#include "wav_io.h"

int embed_data(WavHeader* header, Arguments* arguments) {
    FILE* ptr = open_file(arguments->p_wavefile, "rb");
    FILE* ptr_write = open_file(arguments->out_file, "wb");
    FILE* ptr_in_data = open_file(arguments->in_file, "rb");

    wav_header_read(header, ptr);
    wav_header_write(header, ptr_write);

    char *ext = get_filename_ext(arguments->in_file);

    if (arguments->encryption.password != NULL) {
        FILE *tmp = tmpfile();

        char plaintext[BLOCK_SIZE];
        char ciphertext[BLOCK_SIZE * 2];

        crypto_setup();

        CipherContext *ctx = crypto_encrypt_init(&(arguments->encryption));

        int len, read;

        unsigned long in_size = get_file_size(ptr_in_data);
        unsigned char in_size_vec[4];
        memset(in_size_vec, 0, 4);
        dec_to_num_representation(in_size, in_size_vec, 4);

        len = crypto_encrypt_update(ctx, in_size_vec, sizeof(in_size_vec), ciphertext);
        fwrite(ciphertext, len, 1, tmp);

        while ((read = fread(plaintext, 1, BLOCK_SIZE, ptr_in_data)) > 0) {
            len = crypto_encrypt_update(ctx, plaintext, read, ciphertext);
            fwrite(ciphertext, len, 1, tmp);
        }

        len = crypto_encrypt_update(ctx, ext, strlen(ext) + 1, ciphertext);
        fwrite(ciphertext, len, 1, tmp);

        len = crypto_encrypt_final(ctx, ciphertext);
        fwrite(ciphertext, len, 1, tmp);

        crypto_teardown();

        fclose(ptr_in_data);
        fseek(tmp, 0, SEEK_SET);

        ptr_in_data = tmp;
    }

    int ret = wav_stego_encode(header, ptr_write, ptr_in_data, arguments->steg, ext);
    if (ret == -1) {
        fprintf(stderr, "Error while encoding file.\n");
        exit(1);
    }

    fclose(ptr);
    fclose(ptr_write);
    fclose(ptr_in_data);

    return 0;
}

int extract_data(WavHeader* header, Arguments* arguments) {
    FILE* ptr = open_file(arguments->p_wavefile, "rb");
    FILE* ptr_write = open_file(arguments->out_file, "wb");
    FILE *ptr_out, *ptr_decrypted;

    wav_header_read(header, ptr);

    char *ext = NULL;

    if (arguments->encryption.password != NULL) {
        ptr_out = ptr_write;
        ptr_decrypted = tmpfile();
        ptr_write = tmpfile();
    } else {
        ext = malloc(MAX_EXTENSION_SIZE);
    }

    int ret = wav_stego_decode(header, ptr_write, arguments->steg, ext);
    if (ret == -1) {
        fprintf(stderr, "Error while decoding file.\n");
        return -1;
    }

    if (arguments->encryption.password != NULL) {
        fseek(ptr_write, 0, SEEK_SET);

        char ciphertext[BLOCK_SIZE];
        char decryptedtext[BLOCK_SIZE];

        crypto_setup();

        CipherContext *ctx = crypto_decrypt_init(&(arguments->encryption));

        int len, read;

        while ((read = fread(ciphertext, 1, BLOCK_SIZE, ptr_write)) > 0) {
            len = crypto_decrypt_update(ctx, ciphertext, read, decryptedtext);
            fwrite(decryptedtext, len, 1, ptr_decrypted);
        }

        len = crypto_decrypt_final(ctx, decryptedtext);
        fwrite(decryptedtext, len, 1, ptr_decrypted);

        crypto_teardown();

        fseek(ptr_decrypted, 0, SEEK_SET);

        unsigned char length_vec[4];
        fread(length_vec, 4, 1, ptr_decrypted);
        unsigned long length = num_representation_to_dec(length_vec, 4);

        int block_size = BLOCK_SIZE;
        while (length > 0) {
            if (length < BLOCK_SIZE) block_size = length;
            fread(decryptedtext, 1, block_size, ptr_decrypted);
            fwrite(decryptedtext, block_size, 1, ptr_out);
            length -= block_size;
        }

        ext = malloc(MAX_EXTENSION_SIZE);
        fread(ext, 1, MAX_EXTENSION_SIZE, ptr_decrypted);

        fclose(ptr_decrypted);
        fclose(ptr_out);
    }

    rename_file_with_extension(arguments->out_file, ext);
    free(ext);

    fclose(ptr);
    fclose(ptr_write);

    return 0;
}

int main(int argc, char** argv) {
    Arguments arguments;

    args_init(&arguments);
    args_parse(argc, argv, &arguments);

    WavHeader header;
    memset(&header, 0, sizeof(header));

    if (arguments.mode == EMBED) {
        embed_data(&header, &arguments);
    } else if (arguments.mode == EXTRACT) {
        extract_data(&header, &arguments);
    } else {
        usage();
    }

    return 0;
}
