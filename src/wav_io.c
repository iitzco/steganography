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

long get_size_of_each_sample(WavHeader* header) {
    return (header->channels * header->bits_per_sample) / 8;
}

long get_num_samples(WavHeader* header) {
    return (8 * header->data_size) / (header->channels * header->bits_per_sample);
}

int wav_header_read(WavHeader* header, FILE* ptr) {
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

int wav_header_write(WavHeader* header, FILE* ptr) {
    int write = 0;

    write = fwrite(&header->native_header, HEADER_SIZE, 1, ptr);

    if (write < 0) {
        printf("Error writing headers\n");
        return -1;
    }
    return 0;
}

int wav_stego_read_bytes_lsbe(int amount, char* carrier, FILE* ptr) {
    unsigned char sample[1];
    int read = 0;
    for (int i = 0; i < amount;) {
        read = fread(sample, 1, 1, ptr);
        if (read == 0) {
            return -1;
        }
        if (sample[0] == 254 || sample[0] == 255) {
            carrier[i] = sample[0];
            i++;
        }
    }
    return 0;
}

int wav_stego_write_bytes_lsbe(int amount, char* msg, FILE* from, FILE* to) {
    unsigned char sample[1];
    int read = 0;
    for (int i = 0; i < amount;) {
        read = fread(sample, 1, 1, from);
        if (read == 0) {
            return -1;
        }
        if (sample[0] == 254 || sample[0] == 255) {
            fwrite(&(msg[i]), 1, 1, to);
            i++;
        } else {
            fwrite(sample, 1, 1, to);
        }
    }
    return 0;
}

void fill_with_ones(unsigned char* vec, int size) {
    for (int i = 0; i < size; i++) {
        vec[i] = 255;
    }
}

int wav_stego_encode(WavHeader* header, FILE* ptr, FILE* msg, StegMode mode, char* ext) {
    char sample_size = header->bits_per_sample / 8;
    char block_byte_size = 0;

    if (mode == LSB1) {
        block_byte_size = sample_size * 8;
    } else if (mode == LSB4) {
        block_byte_size = sample_size * 2;
    } else if (mode == LSBE) {
        block_byte_size = 8;
        sample_size = 1;
    }

    StegMode aux_mode = mode;
    if (mode == LSBE) {
        aux_mode = LSB1;
    }

    // First, write length

    char sample_for_size[block_byte_size * 4];

    unsigned char* length_representation = (unsigned char*)calloc(4, 1);
    unsigned long length = get_file_size(msg);
    dec_to_num_representation(length, length_representation, 4);

    int read = 0;

    if (mode == LSBE) {
        fill_with_ones((unsigned char*)sample_for_size, block_byte_size * 4);
    } else {
        read = fread(sample_for_size, block_byte_size, 4, header->ptr);
    }

    lsb_encode(sample_for_size, block_byte_size * 4, 0, sample_size, (char*)length_representation,
               4, aux_mode);

    if (mode == LSBE) {
        int ret = wav_stego_write_bytes_lsbe(32, sample_for_size, header->ptr, ptr);
        if (ret == -1) return -1;
    } else {
        fwrite(sample_for_size, block_byte_size * 4, 1, ptr);
    }
    // The write input file

    char sample[block_byte_size * BLOCK_SIZE];
    char* block = (char*)calloc(BLOCK_SIZE, 1);
    int block_size = BLOCK_SIZE;
    while (length > 0) {
        if (length < BLOCK_SIZE) block_size = length;
        fread(block, block_size, 1, msg);
        if (mode == LSBE) {
            fill_with_ones((unsigned char*)sample, block_byte_size * block_size);
        } else {
            read = fread(sample, block_byte_size, block_size, header->ptr);
        }
        lsb_encode(sample, block_byte_size * block_size, 0, sample_size, block, block_size,
                   aux_mode);
        if (mode == LSBE) {
            int ret =
                wav_stego_write_bytes_lsbe(block_byte_size * block_size, sample, header->ptr, ptr);
            if (ret == -1) return -1;
        } else {
            fwrite(sample, block_byte_size * block_size, 1, ptr);
        }
        length -= block_size;
    }
    free(block);

    // If needed, write extension

    int len = 0, full_len = 0;  // ATTENTION, must be NULL terminated
    if (ext != NULL) {
        len = strlen(ext);  // ATTENTION, must be NULL terminated
        full_len = len + 1;
        char* sample_ext = (char*)calloc(block_byte_size * full_len, 1);
        if (mode == LSBE) {
            fill_with_ones((unsigned char*)sample_ext, block_byte_size * full_len);
        } else {
            read = fread(sample_ext, block_byte_size, full_len, header->ptr);
        }
        lsb_encode(sample_ext, block_byte_size * full_len, 0, sample_size, ext, full_len, aux_mode);
        if (mode == LSBE) {
            int ret = wav_stego_write_bytes_lsbe(block_byte_size * full_len, sample_ext,
                                                 header->ptr, ptr);
            if (ret == -1) return -1;
        } else {
            fwrite(sample_ext, block_byte_size * full_len, 1, ptr);
        }
        free(sample_ext);
    }

    // Finally write rest of file

    char byte[1];

    while (fread(byte, 1, 1, header->ptr) == 1) {
        fwrite(byte, 1, 1, ptr);
    }

    return 0;
}

int wav_stego_decode(WavHeader* header, FILE* output, StegMode mode, char* ext) {
    char sample_size = header->bits_per_sample / 8;
    int block_byte_size = 0;

    if (mode == LSB1) {
        block_byte_size = sample_size * 8;
    } else if (mode == LSB4) {
        block_byte_size = sample_size * 2;
    } else if (mode == LSBE) {
        block_byte_size = 8;
        sample_size = 1;
    }

    StegMode aux_mode = mode;
    if (mode == LSBE) {
        aux_mode = LSB1;
    }

    // First, read length

    char sample_for_size[block_byte_size * 4];
    unsigned char* length_representation = (unsigned char*)calloc(4, 1);
    unsigned long length;
    int read = 0;

    if (mode == LSBE) {
        int ret = wav_stego_read_bytes_lsbe(32, sample_for_size, header->ptr);
        if (ret == -1) return -1;
    } else {
        read = fread(sample_for_size, block_byte_size, 4, header->ptr);
    }

    lsb_decode(sample_for_size, block_byte_size * 4, 0, sample_size, (char*)length_representation,
               4, aux_mode);
    // Decoded data is in little endian
    length = num_representation_to_dec(length_representation, 4);

    // Then, decode file

    int block_size = BLOCK_SIZE;
    char* block = (char*)malloc(block_size);
    char* sample = (char*)malloc(block_byte_size * block_size);
    while (length > 0) {
        if (length < BLOCK_SIZE) block_size = length;
        if (mode == LSBE) {
            int ret = wav_stego_read_bytes_lsbe(block_byte_size * block_size, sample, header->ptr);
            if (ret == -1) return -1;
        } else {
            read = fread(sample, block_byte_size, block_size, header->ptr);
        }
        lsb_decode(sample, block_byte_size * block_size, 0, sample_size, block, block_size,
                   aux_mode);
        fwrite(block, block_size, 1, output);
        length -= block_size;
    }
    free(block);
    free(sample);

    /* // Finally, get extension if needed */

    if (ext != NULL) {
        int i = 0;
        sample = (char*)malloc(block_byte_size);
        do {
            if (mode == LSBE) {
                int ret = wav_stego_read_bytes_lsbe(block_byte_size, sample, header->ptr);
                if (ret == -1) return -1;
            } else {
                read = fread(sample, block_byte_size, 1, header->ptr);
            }
            lsb_decode(sample, block_byte_size, 0, sample_size, &(ext[i]), 1, aux_mode);
        } while (ext[i++] != 0);
        free(sample);
    }

    return 0;
}
