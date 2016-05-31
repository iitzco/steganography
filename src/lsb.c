#include "lsb.h"

typedef struct {
    char mask;
    char ioff;
    char doff;
} lsb_params_t;

static const lsb_params_t _LSB = {.mask = 0x01, .ioff = 7, .doff = 1};
static const lsb_params_t _LSB4 = {.mask = 0x0F, .ioff = 4, .doff = 4};

void _lsb_encode(lsb_params_t params, char* carrier, size_t carrier_size, size_t offset,
                 size_t chunk_size, char* msg, size_t msg_size) {
    char mask = params.mask;  // change to 0x07
    char off = params.ioff;

    for (int i = offset + chunk_size - 1, j = 0; i < carrier_size && j < msg_size;
         i += chunk_size) {
        char msgbit = (msg[j] >> off) & mask;

        carrier[i] &= ~mask;
        carrier[i] |= msgbit;

        off -= params.doff;
        if (off < 0) {
            off += 8;
            j++;
        }
    }
}

void _lsb_decode(lsb_params_t params, char* carrier, size_t carrier_size, size_t offset,
                 size_t chunk_size, char* msg, size_t msg_size) {
    char mask = params.mask;
    char off = params.ioff;

    for (int i = offset + chunk_size - 1, j = 0; i < carrier_size && j < msg_size;
         i += chunk_size) {
        char msgbit = carrier[i] & mask;
        msg[j] &= (mask << off) ^ 0xFF;
        msg[j] |= msgbit << off;

        off -= params.doff;
        if (off < 0) {
            off += 8;
            j++;
        }
    }
}

void lsb_encode(char* carrier, size_t carrier_size, size_t offset, size_t chunk_size, char* msg,
                size_t msg_size, StegMode mode) {
    if (mode == LSB1) {
        _lsb_encode(_LSB, carrier, carrier_size, offset, chunk_size, msg, msg_size);
    } else if (mode == LSB4) {
        _lsb_encode(_LSB4, carrier, carrier_size, offset, chunk_size, msg, msg_size);
    }
}

void lsb_decode(char* carrier, size_t carrier_size, size_t offset, size_t chunk_size, char* msg,
                size_t msg_size, StegMode mode) {
    if (mode == LSB1) {
        _lsb_decode(_LSB, carrier, carrier_size, offset, chunk_size, msg, msg_size);
    } else if (mode == LSB4) {
        _lsb_decode(_LSB4, carrier, carrier_size, offset, chunk_size, msg, msg_size);
    }
}

/* void lsb_encode(char *carrier, size_t carrier_size, size_t offset, size_t
 * chunk_size, char *msg, size_t msg_size) { */
/*   _lsb_encode(_LSB, carrier, carrier_size, offset, chunk_size, msg,
 * msg_size); */
/* } */

/* void lsb_decode(char *carrier, size_t carrier_size, size_t offset, size_t
 * chunk_size, char *msg, size_t msg_size) { */
/*   _lsb_decode(_LSB, carrier, carrier_size, offset, chunk_size, msg,
 * msg_size); */
/* } */

/* void lsb4_encode(char *carrier, size_t carrier_size, size_t offset, size_t
 * chunk_size, char *msg, size_t msg_size) { */
/*   _lsb_encode(_LSB4, carrier, carrier_size, offset, chunk_size, msg,
 * msg_size); */
/* } */

/* void lsb4_decode(char *carrier, size_t carrier_size, size_t offset, size_t
 * chunk_size, char *msg, size_t msg_size) { */
/*   _lsb_decode(_LSB4, carrier, carrier_size, offset, chunk_size, msg,
 * msg_size); */
/* } */

/* int main() { */
/*   char carrier[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 * 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; */
/*   char msg[1] = {0xFF}; */

/*   lsb4_encode(carrier, 16, 0, 2, msg, 1); */
/*   lsb4_decode(carrier, 16, 0, 2, msg, 1); */

/*   for (int i = 0; i < 16; i++) { */
/*     printf("%hhX ", carrier[i]); */
/*   } */
/*   printf("\n"); */

/*   printf("%hhX\n", msg[0]); */
/* } */
