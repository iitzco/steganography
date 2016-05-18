#include <stdio.h>
#include <string.h>
#include "wav_parser.h"
#include "utils.h"
#include "printer.h"

/* function to show bytes in memory, from location start to start+n*/
void print_mem_rep(char *start, int n) {
    for (int i = 0; i < n; i++) {
        printf(" %.2hhx",start[i]);
    }   
    printf("\n");
}

void print_extra_data(HEADER_PARSED *header_p) {
    printf("EXTRA DATA\n");
    // calculate no.of samples
    long num_samples = get_num_samples(header_p);
    printf("Number of samples:%lu \n", num_samples);

    long size_of_each_sample = get_size_of_each_sample(header_p);
    printf("Size of each sample:%ld bytes\n", size_of_each_sample);

    // calculate duration of file
    float duration_in_seconds = (float)header_p->overall_size /header_p->byterate;
    printf("Approx.Duration in seconds=%f\n", duration_in_seconds);
    printf("Approx.Duration in h:m:s=%s\n", seconds_to_time(duration_in_seconds));
}

void print_all_headers(HEADER *header) {

    printf("RIFF HEADER\n");
    printf("parsed -> %.4s\n", header->header_p.riff);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.riff_desc.chunkID),sizeof(CKID));
    printf("\n");

    printf("OVERALL SIZE HEADER\n");
    printf("parsed -> %d\n", header->header_p.overall_size);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.riff_desc.chunkSize), sizeof(CKSIZE));
    printf("\n");

    printf("FORMAT HEADER\n");
    printf("parsed -> %.4s\n", header->header_p.wave);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.riff_desc.format),sizeof(CKID));
    printf("\n");

    printf("FMT HEADER\n");
    printf("parsed -> %s\n", header->header_p.fmt_chunk_marker);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.chunkID),sizeof(CKID));
    printf("\n");

    printf("FORMAT CATEGORY (16 for PCM)\n");
    printf("parsed -> %d\n", header->header_p.length_of_fmt);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.chunkSize), sizeof(CKSIZE));
    printf("\n");

    char format_name[10] = "";
    if (header->header_p.format_type == 1)
      strcpy(format_name,"PCM");
    else if (header->header_p.format_type == 6)
     strcpy(format_name, "A-law");
    else if (header->header_p.format_type == 7)
     strcpy(format_name, "Mu-law");

    printf("AUDIO FORMAT CATEGORY (1 for PCM)\n");
    printf("parsed -> %d (%s)\n", header->header_p.format_type, format_name);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.wFormatTag), sizeof(WORD));
    printf("\n");

    printf("AUDIO CHANNEL\n");
    printf("parsed -> %d \n", header->header_p.channels);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.wChannels), sizeof(WORD));
    printf("\n");

    printf("AUDIO SAMPLE RATE\n");
    printf("parsed -> %d \n", header->header_p.sample_rate);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.dwSamplesPerSec), sizeof(DWORD));
    printf("\n");

    printf("AUDIO BYTE RATE\n");
    printf("parsed -> %d \n", header->header_p.byterate);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.dwAvgBytesPerSec), sizeof(DWORD));
    printf("\n");

    printf("BLOCK ALIGNMENT\n");
    printf("parsed -> %d \n", header->header_p.block_align);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.wBlockAlign), sizeof(WORD));
    printf("\n");

    printf("BYTES PER SAMPLE\n");
    printf("parsed -> %d \n", header->header_p.bits_per_sample);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.fmt.wBitsPerSample), sizeof(WORD));
    printf("\n");

    printf("DATA HEADER\n");
    printf("parsed -> %s\n", header->header_p.data_chunk_header);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.data.chunkID),sizeof(CKID));
    printf("\n");

    printf("DATA SIZE\n");
    printf("parsed -> %d \n", header->header_p.data_size);
    printf("native -> ");
    print_mem_rep((char *)&(header->header_n.data.chunkSize), sizeof(CKSIZE));
    printf("\n");


}

void print_sound_data(HEADER *header) {

    int read = 0;
    long size_of_each_sample = get_size_of_each_sample(&header->header_p);
    long num_samples = get_num_samples(&header->header_p);

    // read each sample from data chunk if PCM
    if (header->header_p.format_type == 1) { // PCM
       printf("Dump sample data? Y/N?");
       char c = 'n';
       scanf("%c", &c);
       if (c == 'Y' || c == 'y') {
           long i =0;
           char data_buffer[size_of_each_sample];
           int  size_is_correct = TRUE;

           // make sure that the bytes-per-sample is completely divisible by num.of channels
           long bytes_in_each_channel = (size_of_each_sample / header->header_p.channels);
           if ((bytes_in_each_channel  * header->header_p.channels) != size_of_each_sample) {
               printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header->header_p.channels, size_of_each_sample);
               size_is_correct = FALSE;
           }

           if (size_is_correct) {
                       // the valid amplitude range for values based on the bits per sample
               long low_limit = 0l;
               long high_limit = 0l;

               switch (header->header_p.bits_per_sample) {
                   case 8:
                       low_limit = -128;
                       high_limit = 127;
                       break;
                   case 16:
                       low_limit = -32768;
                       high_limit = 32767;
                       break;
                   case 32:
                       low_limit = -2147483648;
                       high_limit = 2147483647;
                       break;
               }

               printf("\n\n.Valid range for data values : %ld to %ld \n", low_limit, high_limit);
               for (i =1; i <= num_samples; i++) {
                   printf("==========Sample %ld / %ld=============\n", i, num_samples);
                   read = fread(data_buffer, sizeof(data_buffer), 1, header->header_p.ptr);
                   if (read == 1) {
                   
                       // dump the data read
                       unsigned int  xchannels = 0;
                       int data_in_channel = 0;

                       for (xchannels = 0; xchannels < header->header_p.channels; xchannels ++ ) {
                           printf("Channel#%d : ", (xchannels+1));
                           // convert data from little endian to big endian based on bytes in each channel sample
                           if (bytes_in_each_channel == 4) {
                               data_in_channel =   data_buffer[0] |
                                                   (data_buffer[1]<<8) |
                                                   (data_buffer[2]<<16) |
                                                   (data_buffer[3]<<24);
                           }
                           else if (bytes_in_each_channel == 2) {
                               data_in_channel = data_buffer[0] |
                                                   (data_buffer[1] << 8);
                           }
                           else if (bytes_in_each_channel == 1) {
                               data_in_channel = data_buffer[0];
                           }

                           printf("%d ", data_in_channel);

                           // check if value was in range
                           if (data_in_channel < low_limit || data_in_channel > high_limit)
                               printf("**value out of range\n");

                           printf(" | ");
                       }

                       printf("\n");
                   }
                   else {
                       printf("Error reading file. %d bytes\n", read);
                       break;
                   }
               }
           }
        }
    }
}
