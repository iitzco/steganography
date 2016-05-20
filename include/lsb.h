#ifndef LSB_H
#define LSB_H

#include <stdio.h>

typedef enum t_steg { S_NONE, LSB1, LSB4, LSBE } Steg;

void lsb_encode(char *carrier, size_t carrier_size, size_t offset,
                size_t chunk_size, char *msg, size_t msg_size, int mode);

void lsb_decode(char *carrier, size_t carrier_size, size_t offset,
                size_t chunk_size, char *msg, size_t msg_size, int mode);

#endif
