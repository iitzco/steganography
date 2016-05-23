#ifndef LSB_H
#define LSB_H

#include <stdio.h>

typedef enum { S_NONE, LSB1, LSB4, LSBE } StegMode;

void lsb_encode(char* carrier, size_t carrier_size, size_t offset,
                size_t chunk_size, char* msg, size_t msg_size, StegMode mode);

void lsb_decode(char* carrier, size_t carrier_size, size_t offset,
                size_t chunk_size, char* msg, size_t msg_size, StegMode mode);

#endif
