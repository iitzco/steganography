#ifndef LSB_H
#define LSB_H

void lsb_encode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size);

void lsb_decode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size);

void lsb4_encode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size);

void lsb4_decode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size);

#endif
