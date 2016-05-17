// WAVE file header format
// Source: http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/

typedef unsigned int DWORD; // 4bytes
typedef unsigned char BYTE; // 1byte
typedef unsigned short  WORD; // 2bytes

typedef DWORD FOURCC; //Four-character code
typedef FOURCC CKID; //Four-character-code chunk identifier
typedef DWORD CKSIZE; // 32-bit unsigned size value


typedef struct {
    unsigned char riff[4];                      // RIFF string
    unsigned int overall_size;                  // overall size of file in bytes
    unsigned char wave[4];                      // WAVE string
    unsigned char fmt_chunk_marker[4];          // fmt string with trailing null char
    unsigned int length_of_fmt;                 // length of the format data
    unsigned int format_type;                   // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
    unsigned int channels;                      // no.of channels
    unsigned int sample_rate;                   // sampling rate (blocks per second)
    unsigned int byterate;                      // SampleRate * NumChannels * BitsPerSample/8
    unsigned int block_align;                   // NumChannels * BitsPerSample/8
    unsigned int bits_per_sample;               // bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header [4];        // DATA string or FLLR string
    unsigned int data_size;                     // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
    FILE * ptr;                                 // sound data FILE pointer
} HEADER_PARSED;

typedef struct{
    CKID chunkID; // 'RIFF'
    CKSIZE chunkSize; // File format
    CKID format; // Format: 'WAVE'
} RIFF_CK;

typedef struct{
    CKID chunkID; //'fmt '
    CKSIZE chunkSize; // 16 para PCM.Size of rest of subchunk.
    WORD wFormatTag; // Format category,i.e.:PCM = 1 (no compres.)
    WORD wChannels; // Number of channels:1, mono; 2, stereo
    DWORD dwSamplesPerSec; // Sampling rate: Mhz
    DWORD dwAvgBytesPerSec;
    WORD wBlockAlign;
    WORD wBitsPerSample; //8, 16, etc.
} FMT_CK;

typedef struct{
    CKID chunkID; // 'data'
    CKSIZE chunkSize; // Bytes of data
    BYTE *soundData; // Sound data.
} DATA_CK;

typedef struct
{
    RIFF_CK riff_desc; // MANDATORY
    FMT_CK fmt; // Format Chunk MANDATORY
    DATA_CK data; // Wave Data Chunk MANDATORY
} HEADER_NATIVE;


typedef struct {
    HEADER_PARSED header_p;
    HEADER_NATIVE header_n;
} HEADER;


long get_size_of_each_sample(HEADER_PARSED *header_p);

long get_num_samples(HEADER_PARSED *header_p);
