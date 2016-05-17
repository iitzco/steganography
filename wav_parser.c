// /**
//  * Read and parse a wave file
//  *
//  * Source: http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
//  **/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wav_parser.h"
#include "utils.h"
#include "printer.h"


unsigned char buffer4[4];
unsigned char buffer2[2];


long get_size_of_each_sample(HEADER_PARSED *header_p) {
    return (header_p->channels * header_p->bits_per_sample) / 8;
}

long get_num_samples(HEADER_PARSED *header_p) {
    return (8 *header_p->data_size) / (header_p->channels * header_p->bits_per_sample);

}

int main(int argc, char **argv) {

    FILE *ptr;
    
    //Initialize in 0
    HEADER header;
    memset(&header, 0, sizeof(header));

    char *filename = get_file_path(argc, argv);
    if (filename==NULL) {
        exit(1);
    }

    // open file
    ptr = fopen(filename, "rb");
    if (ptr == NULL) {
       printf("Error opening file\n");
       exit(1);
    }

    int read = 0;

    /* ------------------------ RIFF Chunk Descriptor ------------------------ */

    // Read RIFF chunkID

    read = fread(header.header_p.riff, sizeof(header.header_p.riff), 1, ptr);
    memcpy(&header.header_n.riff_desc.chunkID,header.header_p.riff,sizeof(CKID));

    // Read RIFF ChunkSize

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    memcpy(&header.header_n.riff_desc.chunkSize,buffer4,sizeof(CKSIZE));

    // convert little endian to big endian 4 byte int
    header.header_p.overall_size  = little_to_big_4_bytes(buffer4); 

    // Read format ('WAVE')

    read = fread(header.header_p.wave, sizeof(header.header_p.wave), 1, ptr);
    memcpy(&header.header_n.riff_desc.format, header.header_p.wave, sizeof(CKID));

    /* ---------------------------------------------------------------------------- */
    /* ------------------------ FMT sub-chunk ------------------------ */

    // Read 'fmt '

    read = fread(header.header_p.fmt_chunk_marker, sizeof(header.header_p.fmt_chunk_marker), 1, ptr);
    memcpy(&header.header_n.fmt.chunkID, header.header_p.fmt_chunk_marker, sizeof(CKID));

    // Read subchunkSize

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    memcpy(&header.header_n.fmt.chunkSize,buffer4,sizeof(CKSIZE));

    // convert little endian to big endian 4 byte integer
    header.header_p.length_of_fmt = little_to_big_4_bytes(buffer4);

    // Read Audio Format

    read = fread(buffer2, sizeof(buffer2), 1, ptr);
    memcpy(&header.header_n.fmt.wFormatTag, buffer2, sizeof(WORD));
    header.header_p.format_type = little_to_big_2_bytes(buffer2);

    // Read Audio Channels

    read = fread(buffer2, sizeof(buffer2), 1, ptr);
    memcpy(&header.header_n.fmt.wChannels, buffer2, sizeof(WORD));
    header.header_p.channels = little_to_big_2_bytes(buffer2);

    // Read Sample Rate

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    memcpy(&header.header_n.fmt.dwSamplesPerSec, buffer4, sizeof(DWORD));

    header.header_p.sample_rate = little_to_big_4_bytes(buffer4);

    // Read Byte Rate

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    memcpy(&header.header_n.fmt.dwAvgBytesPerSec, buffer4, sizeof(DWORD));

    header.header_p.byterate  = little_to_big_4_bytes(buffer4);

    // Read Block Alignment

    read = fread(buffer2, sizeof(buffer2), 1, ptr);
    memcpy(&header.header_n.fmt.wBlockAlign, buffer2, sizeof(WORD));

    header.header_p.block_align = little_to_big_2_bytes(buffer2);

    // Read Bits Per Sample

    read = fread(buffer2, sizeof(buffer2), 1, ptr);
    memcpy(&header.header_n.fmt.wBitsPerSample, buffer2, sizeof(WORD));

    header.header_p.bits_per_sample = little_to_big_2_bytes(buffer2);

    /* ---------------------------------------------------------------------------- */
    /* ------------------------ DATA sub-chunk Descriptor ------------------------ */

    // Read Data marker

    read = fread(header.header_p.data_chunk_header, sizeof(header.header_p.data_chunk_header), 1, ptr);
    memcpy(&header.header_n.data.chunkID,header.header_p.data_chunk_header, sizeof(CKID));

    // Read Size of data

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    memcpy(&header.header_n.data.chunkSize, buffer4, sizeof(CKSIZE));

    header.header_p.data_size = little_to_big_4_bytes(buffer4);

    /* ---------------------------------------------------------------------------- */
    //Asign FILE pointer to HEADER struct

    header.header_p.ptr = ptr;

    /* ---------------------------------------------------------------------------- */

    print_all_headers(&header);
    print_extra_data(&header.header_p);

    //Careful, moves ptr (reads from file)
    print_sound_data(&header);

    printf("Closing file..\n");
    fclose(ptr);

     // cleanup before quitting
    free(filename);
    return 0;

}
