#ifndef LSB_H
#define LSB_H

#define LSB1 0
#define LSB4 1

void lsb_encode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size, int mode);

void lsb_decode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size, int mode);

#endif
