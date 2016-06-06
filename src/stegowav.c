#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "arguments.h"
#include "encrypt.h"
#include "lsb.h"
#include "printer.h"
#include "utils.h"
#include "wav_io.h"

int embed_data(WavHeader* header, Arguments* arguments) {
    FILE* ptr = open_file(arguments->p_wavefile, "r");
    FILE* ptr_write = open_file(arguments->out_file, "w");
    FILE* ptr_in_data = open_file(arguments->in_file, "r");

    wav_header_read(header, ptr);
    wav_header_write(header, ptr_write);

    char *ext = NULL;

    if (arguments->encryption.algorithm != 0) {
        FILE *tmp = tmpfile();

        char plaintext[BLOCK_SIZE];
        char ciphertext[BLOCK_SIZE * 2];

        crypto_setup();

        CipherContext *ctx = crypto_encrypt_init(&(arguments->encryption));

        int len;

        while (fread(plaintext, BLOCK_SIZE, 1, ptr_in_data) > 0) {
            len = crypto_encrypt_update(ctx, plaintext, BLOCK_SIZE, ciphertext);
            fwrite(ciphertext, len, 1, tmp);
        }

        len = crypto_encrypt_final(ctx, ciphertext);
        fwrite(ciphertext, len, 1, tmp);

        crypto_teardown();

        fclose(ptr_in_data);
        fseek(tmp, 0, SEEK_SET);

        ptr_in_data = tmp;

    } else {
        ext = get_filename_ext(arguments->in_file);
    }

    int ret = wav_stego_encode(header, ptr_write, ptr_in_data, arguments->steg, ext);
    if (ret == -1) {
        printf("Error while encoding file.\n");
    } else {
        printf("Encoding %s\n", ext);
    }

    fclose(ptr);
    fclose(ptr_write);
    fclose(ptr_in_data);

    return 0;
}

int extract_data(WavHeader* header, Arguments* arguments) {
    FILE* ptr = open_file(arguments->p_wavefile, "r");
    FILE* ptr_write = open_file(arguments->out_file, "w");
    FILE *ptr_out;

    wav_header_read(header, ptr);

    char *ext = NULL;

    if (arguments->encryption.algorithm != 0) {
        ptr_out = ptr_write;
        ptr_write = tmpfile();
    } else {
        ext = (char*)calloc(MAX_EXTENSION_SIZE, 1);
    }

    int ret = wav_stego_decode(header, ptr_write, arguments->steg, ext);
    if (ret == -1) {
        printf("Error while decoding file.\n");
        return -1;
    }

    if (arguments->encryption.algorithm != 0) {
        // DEBUG: print ciphertext
        int ciphertext_len = ftell(ptr_write);
        printf("len: %d\n", ciphertext_len);
        fseek(ptr_write, 0, SEEK_SET);

        char *buf = (char *)malloc(ciphertext_len);
        fread(buf, ciphertext_len, 1, ptr_write);
        BIO_dump_fp(stdout, (const char *)buf, ciphertext_len);
        free(buf);
        // END DEBUG

        fseek(ptr_write, 0, SEEK_SET);

        char ciphertext[BLOCK_SIZE];
        char decryptedtext[BLOCK_SIZE];

        crypto_setup();

        CipherContext *ctx = crypto_decrypt_init(&(arguments->encryption));

        int len;

        while (fread(ciphertext, BLOCK_SIZE, 1, ptr_write) > 0) {
            len = crypto_decrypt_update(ctx, ciphertext, BLOCK_SIZE, decryptedtext);
            fwrite(decryptedtext, len, 1, ptr_out);
        }

        len = crypto_decrypt_final(ctx, decryptedtext);
        fwrite(decryptedtext, len, 1, ptr_out);

        crypto_teardown();

        fclose(ptr_out);

    } else {
        rename_file_with_extension(arguments->out_file, ext);
    }

    fclose(ptr);
    fclose(ptr_write);

    return 0;
}

int main(int argc, char** argv) {
    Arguments arguments;

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
