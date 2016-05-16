// WAVE file header format
// Source: http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/

typedef unsigned char BYTE;

typedef struct {
    unsigned char riff[4];                      // RIFF string
    unsigned int overall_size   ;               // overall size of file in bytes
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
} HEADER;

typedef struct {
    HEADER *header;
    BYTE *soundData;
} WAV;
/*
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef DWORD FOURCC; //Four-character code
typedef FOURCC CKID;
typedef DWORD CKSIZE; //Four-character-code chunk i


struct wavStr
{
    RIFF_CK riff_desc; // MANDATORY
    FMT_CK fmt; // Format Chunk MANDATORY
    DATA_CK data; // Wave Data Chunk MANDATORY
};

typedef struct{
    CKID chunkID;
    CKSIZE chunkSize;
    CKID format;
} RIFF_CK;

typedef struct{
    CKID chunkID; //'fmt '
    CKSIZE chunkSize; // 16 para PCM.Size of rest of subchunk.
    WORD wFormatTag; // Format category,i.e.:PCM = 1 (no compres.)
    WORD wChannels; // Number of channels:1, mono; 2, stereo
    DWORD dwSamplesPerSec; // Sampling rate: Mhz
    DWORD dwAvgBytesPerSec;
    WORD wBlockAlign
    WORD wBitsPerSample; //8, 16, etc.
    WORD extraParamSize;// If PCM, doesn't exist
    BYTE *extraParams;//space for extra params
} FMT_CK;

typedef struct{
CKID chunkID; // 'data'
CKSIZE chunkSize; // Bytes of data
BYTE *soundData; // Sound data.
} DATA_CK;
*/




 