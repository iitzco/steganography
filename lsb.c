#include <stdio.h>

void lsb_encode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size) {
  // TODO: chunk_size handling
  char mask = 0x01;
  char off = 7;

  for (int i = offset, j = 0; i < carrier_size && j < msg_size; i += chunk_size) {
    char msgbit = (msg[j] >> off) & mask;

    carrier[i] &= ~mask;
    carrier[i] |= mask & msgbit;

    off--;
    if (off == -1) {
      off = 7;
      j++;
    }
  }
}

void lsb_decode(char *carrier, size_t carrier_size, size_t offset, size_t chunk_size, char *msg, size_t msg_size) {
  // TODO: chunk_size handling
  char mask = 0x01;
  char off = 7;

  for (int i = offset, j = 0; i < carrier_size && j < msg_size; i += chunk_size) {
    char msgbit = carrier[i] & mask;
    msg[j] |= msgbit << off;

    off--;
    if (off == -1) {
      off = 7;
      j++;
    }
  }
}

int main() {
  char carrier[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  char msg[1] = {0xFF};

  lsb_encode(carrier, 16, 0, 2, msg, 1);
  lsb_decode(carrier, 16, 0, 2, msg, 1);

  for (int i = 0; i < 16; i++) {
    printf("%hhX ", carrier[i]);
  }
  printf("\n");

  printf("%hhX\n", msg[0]);
}
