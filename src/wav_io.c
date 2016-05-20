// /**
//  * Read and parse a wave file
//  *
//  * Source: http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
//  **/
#include "wav_io.h"
#include <stdlib.h>
#include <string.h>
#include "lsb.h"
#include "utils.h"

unsigned char buffer4[4];
unsigned char buffer2[2];

long get_size_of_each_sample(HEADER *header) {
    return (header->channels * header->bits_per_sample) / 8;
}

long get_num_samples(HEADER *header) {
    return (8 * header->data_size) /
           (header->channels * header->bits_per_sample);
}

int wav_header_read(HEADER *header, FILE *ptr) {
    int read = 0;

    // Read full header from ptr
    read = fread(header->native_header, HEADER_SIZE, 1, ptr);

    if (read == 0) {
        printf("Error reading headers\n");
        return -1;
    }

    header->ptr = ptr;

    /* ------------------------ RIFF Chunk Descriptor ------------------------
     */

    // Read RIFF chunkID

    memcpy(header->riff, header->native_header, 4);

    memcpy(buffer4, &(header->native_header[4]), 4);
    header->overall_size = little_to_big_4_bytes(buffer4);

    // Read format ('WAVE')

    memcpy(header->wave, &(header->native_header[8]), 4);

    /* ------------------------ FMT sub-chunk ------------------------ */

    // Read 'fmt '

    memcpy(header->fmt_chunk_marker, &(header->native_header[12]), 4);

    // Read subchunkSize

    memcpy(buffer4, &(header->native_header[16]), 4);
    header->length_of_fmt = little_to_big_4_bytes(buffer4);

    // Read Audio Format

    memcpy(buffer2, &(header->native_header[20]), 2);
    header->format_type = little_to_big_2_bytes(buffer2);

    // Read Audio Channels

    memcpy(buffer2, &(header->native_header[22]), 2);
    header->channels = little_to_big_2_bytes(buffer2);

    // Read Sample Rate

    memcpy(buffer4, &(header->native_header[24]), 4);
    header->sample_rate = little_to_big_4_bytes(buffer4);

    // Read Byte Rate

    memcpy(buffer4, &(header->native_header[28]), 4);
    header->byterate = little_to_big_4_bytes(buffer4);

    // Read Block Alignment

    memcpy(buffer2, &(header->native_header[32]), 2);
    header->block_align = little_to_big_2_bytes(buffer2);

    // Read Bits Per Sample

    memcpy(buffer2, &(header->native_header[34]), 2);
    header->bits_per_sample = little_to_big_2_bytes(buffer2);

    /* ------------------------ DATA sub-chunk Descriptor
     * ------------------------ */

    // Read Data marker

    memcpy(header->data_chunk_header, &(header->native_header[36]), 4);

    // Read Size of data

    memcpy(buffer4, &(header->native_header[40]), 4);
    header->data_size = little_to_big_4_bytes(buffer4);

    return 0;
}

int wav_header_write(HEADER *header, FILE *ptr) {
    int write = 0;

    write = fwrite(&header->native_header, HEADER_SIZE, 1, ptr);

    if (write < 0) {
        printf("Error writing headers\n");
        return -1;
    }
    return 0;
}

int wav_stego_encode(HEADER *header, FILE *ptr, FILE *msg, Steg mode) {
    char sample_size = header->bits_per_sample / 8;
    char block_byte_size = 0;

    if (mode == LSB1) {
        block_byte_size = sample_size * 8;
    } else if (mode == LSB4) {
        block_byte_size = sample_size * 2;
    }

    // First write length

    char sample_for_size[block_byte_size * 4];
    unsigned long length = get_file_size(msg);

    lsb_encode(sample_for_size, block_byte_size * 4, 0, sample_size,
               (char *)&length, 4, mode);
    fwrite(sample_for_size, block_byte_size * 4, 1, ptr);

    // The write input file

    char sample[block_byte_size * BLOCK_SIZE];
    char *block = (char *)calloc(BLOCK_SIZE, 1);
    int read = 0;
    while (length > BLOCK_SIZE) {
        fread(block, BLOCK_SIZE, 1, msg);
        read = fread(sample, block_byte_size, BLOCK_SIZE, header->ptr);
        /* printf("Loop %d and length %lu\n",read,length); */
        lsb_encode(sample, block_byte_size * BLOCK_SIZE, 0, sample_size, block,
                   BLOCK_SIZE, mode);
        fwrite(sample, block_byte_size * BLOCK_SIZE, 1, ptr);
        length -= BLOCK_SIZE;
    }
    fread(block, length, 1, msg);
    read = fread(sample, block_byte_size, length, header->ptr);
    /* printf("%d\n",read); */
    lsb_encode(sample, block_byte_size * length, 0, sample_size, block, length,
               mode);
    fwrite(sample, block_byte_size, length, ptr);

    // Finally write rest of file

    int remain =
        header->data_size - (4 * block_byte_size) - (length * block_byte_size);
    char byte[1];

    while (remain-- > 0) {
        fread(byte, 1, 1, header->ptr);
        fwrite(byte, 1, 1, ptr);
    }

    return 0;
}

int wav_stego_decode(HEADER *header, FILE *output, Steg mode) {
    char sample_size = header->bits_per_sample / 8;
    int block_byte_size = 0;

    if (mode == LSB1) {
        block_byte_size = sample_size * 8;
    } else if (mode == LSB4) {
        block_byte_size = sample_size * 2;
    }

    // First, read length

    char sample_for_size[block_byte_size * 4];
    unsigned long length;
    char *buffer = (char *)calloc(4,1);
    int read = 0;

    read = fread(sample_for_size, block_byte_size, 4, header->ptr);
    lsb_decode(sample_for_size, block_byte_size * 4, 0, sample_size, buffer, 4,
               mode);
    // Decoded data is in little endian
    length = little_to_big_4_bytes((unsigned char *)buffer);

    // Then, decode file

    char *sample = NULL;
    char *block = NULL;
    while (length > BLOCK_SIZE) {
        block = (char *)calloc(BLOCK_SIZE, 1);
        sample = (char *)calloc(block_byte_size * BLOCK_SIZE, 1);
        read = fread(sample, block_byte_size, BLOCK_SIZE, header->ptr);
        lsb_decode(sample, block_byte_size * BLOCK_SIZE, 0, sample_size, block,
                   BLOCK_SIZE, mode);
        fwrite(block, BLOCK_SIZE, 1, output);
        length -= BLOCK_SIZE;
        free(sample);
        free(block);
    }
    block = (char *)calloc(BLOCK_SIZE, 1);
    sample = (char *)calloc(block_byte_size * BLOCK_SIZE, 1);
    read = fread(sample, block_byte_size, length, header->ptr);
    lsb_decode(sample, block_byte_size * length, 0, sample_size, block, length,
               mode);
    read = fwrite(block, 1, length, output);
    free(sample);
    free(block);
    return 0;
}
