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
#define TRUE 1 
#define FALSE 0


unsigned char buffer4[4];
unsigned char buffer2[2];

char* seconds_to_time(float seconds);

/* function to show bytes in memory, from location start to start+n*/
void print_mem_rep(char *start, int n) {
    for (int i = 0; i < n; i++) {
        printf(" %.2hhx",start[i]);
    }   
    printf("\n");
}

char* get_file_path(int argc, char **argv){

    char *filename = (char*) malloc(sizeof(char) * 1024);

    if (filename == NULL) {
      printf("Error in malloc\n");
      return NULL;
    }

    // get file path
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      
       strcpy(filename, cwd);

       // get filename from command line
       if (argc < 2) {
         printf("No wave file specified\n");
         return NULL;
       }
       
       strcat(filename, "/");
       strcat(filename, argv[1]);
       printf("Reading %s\n", filename);
       return filename;
    }
    return NULL;
}

int main(int argc, char **argv) {

    FILE *ptr;
    
    //Initialize in 0
    HEADER header = {0};

    char *filename = get_file_path(argc, argv);
    if (filename==NULL) {
        exit(1);
    }

    // open file
    printf("Opening  file..\n");
    ptr = fopen(filename, "rb");
    if (ptr == NULL) {
       printf("Error opening file\n");
       exit(1);
    }

    int read = 0;

    // Read RIFF chunkID

    read = fread(header.header_p.riff, sizeof(header.header_p.riff), 1, ptr);
    memcpy(&header.header_n.riff_desc.chunkID,header.header_p.riff,sizeof(CKID));
    printf("RIFF HEADER\n");
    printf("parsed -> %s\n", header.header_p.riff);
    printf("native -> ");
    print_mem_rep((char *)&(header.header_n.riff_desc.chunkID),sizeof(CKID));

    // Read RIFF ChunkSize
    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    memcpy(&header.header_n.riff_desc.chunkSize,buffer4,sizeof(CKSIZE));

    // convert little endian to big endian 4 byte int
    header.header_p.overall_size  = buffer4[0] |
                           (buffer4[1]<<8) |
                           (buffer4[2]<<16) |
                           (buffer4[3]<<24);

    printf("OVERALL SIZE HEADER\n");
    printf("parsed -> %d\n", header.header_p.overall_size);
    printf("native -> ");
    print_mem_rep((char *)&(header.header_n.riff_desc.chunkSize), sizeof(CKSIZE));

    read = fread(header.header_p.wave, sizeof(header.header_p.wave), 1, ptr);
    printf("(9-12) Wave marker: %s\n", header.header_p.wave);

    read = fread(header.header_p.fmt_chunk_marker, sizeof(header.header_p.fmt_chunk_marker), 1, ptr);
    printf("(13-16) Fmt marker: %s\n", header.header_p.fmt_chunk_marker);

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    // convert little endian to big endian 4 byte integer
    header.header_p.length_of_fmt = buffer4[0] |
                               (buffer4[1] << 8) |
                               (buffer4[2] << 16) |
                               (buffer4[3] << 24);
    printf("(17-20) Length of Fmt header:header_p. %u \n", header.header_p.length_of_fmt);

    read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u \n", buffer2[0], buffer2[1]);

    header.header_p.format_type = buffer2[0] | (buffer2[1] << 8);
    char format_name[10] = "";
    if (header.header_p.format_type == 1)
      strcpy(format_name,"PCM");
    else if (header.header_p.format_type == 6)
     strcpy(format_name, "A-law");
    else if (header.header_p.format_type == 7)
     strcpy(format_name, "Mu-law");

    printf("(21-22) Format type: %u %s \n", header.header_p.format_type, format_name);

    read = fread(buffer2, sizeof(buffer2), 1, ptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    header.header_p.channels = buffer2[0] | (buffer2[1] << 8);
    printf("(23-24) Channels: %u \n", header.header_p.channels);

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    header.header_p.sample_rate = buffer4[0] |
                           (buffer4[1] << 8) |
                           (buffer4[2] << 16) |
                           (buffer4[3] << 24);

    printf("(25-28) Sample rate: %u\n", header.header_p.sample_rate);

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    header.header_p.byterate  = buffer4[0] |
                           (buffer4[1] << 8) |
                           (buffer4[2] << 16) |
                           (buffer4[3] << 24);
    printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.header_p.byterate, header.header_p.byterate*8);

    read = fread(buffer2, sizeof(buffer2), 1, ptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    header.header_p.block_align = buffer2[0] |
                       (buffer2[1] << 8);
    printf("(33-34) Block Alignment: %u \n", header.header_p.block_align);

    read = fread(buffer2, sizeof(buffer2), 1, ptr);
    printf("%u %u \n", buffer2[0], buffer2[1]);

    header.header_p.bits_per_sample = buffer2[0] |
                       (buffer2[1] << 8);
    printf("(35-36) Bits per sample: %u \n", header.header_p.bits_per_sample);

    read = fread(header.header_p.data_chunk_header, sizeof(header.header_p.data_chunk_header), 1, ptr);
    printf("(37-40) Data Marker: %s \n", header.header_p.data_chunk_header);

    read = fread(buffer4, sizeof(buffer4), 1, ptr);
    printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    header.header_p.data_size = buffer4[0] |
                   (buffer4[1] << 8) |
                   (buffer4[2] << 16) |
                   (buffer4[3] << 24 );
    printf("(41-44) Size of data chunk: %u \n", header.header_p.data_size);


    // calculate no.of samples
    long num_samples = (8 * header.header_p.data_size) / (header.header_p.channels * header.header_p.bits_per_sample);
    printf("Number of samples:%lu \n", num_samples);

    long size_of_each_sample = (header.header_p.channels * header.header_p.bits_per_sample) / 8;
    printf("Size of each sample:%ld bytes\n", size_of_each_sample);

    // calculate duration of file
    float duration_in_seconds = (float) header.header_p.overall_size / header.header_p.byterate;
    printf("Approx.Duration in seconds=%f\n", duration_in_seconds);
    printf("Approx.Duration in h:m:s=%s\n", seconds_to_time(duration_in_seconds));

    // read each sample from data chunk if PCM
    if (header.header_p.format_type == 1) { // PCM
       printf("Dump sample data? Y/N?");
       char c = 'n';
       scanf("%c", &c);
       if (c == 'Y' || c == 'y') {
           long i =0;
           char data_buffer[size_of_each_sample];
           int  size_is_correct = TRUE;

           // make sure that the bytes-per-sample is completely divisible by num.of channels
           long bytes_in_each_channel = (size_of_each_sample / header.header_p.channels);
           if ((bytes_in_each_channel  * header.header_p.channels) != size_of_each_sample) {
               printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.header_p.channels, size_of_each_sample);
               size_is_correct = FALSE;
           }

           if (size_is_correct) {
                       // the valid amplitude range for values based on the bits per sample
               long low_limit = 0l;
               long high_limit = 0l;

               switch (header.header_p.bits_per_sample) {
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
                   read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
                   if (read == 1) {
                   
                       // dump the data read
                       unsigned int  xchannels = 0;
                       int data_in_channel = 0;

                       for (xchannels = 0; xchannels < header.header_p.channels; xchannels ++ ) {
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

    printf("Closing file..\n");
    fclose(ptr);

     // cleanup before quitting
    free(filename);
    return 0;

}

/**
 * Convert seconds into hh:mm:ss format
 * Params:
 *  seconds - seconds value
 * Returns: hms - formatted string
 **/
 char* seconds_to_time(float raw_seconds) {
  char *hms;
  int hours, hours_residue, minutes, seconds, milliseconds;
  hms = (char*) malloc(100);

  sprintf(hms, "%f", raw_seconds);

  hours = (int) raw_seconds/3600;
  hours_residue = (int) raw_seconds % 3600;
  minutes = hours_residue/60;
  seconds = hours_residue % 60;
  milliseconds = 0;

  // get the decimal part of raw_seconds to get milliseconds
  char *pos;
  pos = strchr(hms, '.');
  int ipos = (int) (pos - hms);
  char decimalpart[15];
  memset(decimalpart, ' ', sizeof(decimalpart));
  strncpy(decimalpart, &hms[ipos+1], 3);
  milliseconds = atoi(decimalpart); 

  
  sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
  return hms;
}