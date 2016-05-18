#ifndef PRINTER_H
#define PRINTER_H

#include "wav_io.h"

void print_extra_data(HEADER_PARSED *header_p);

void print_mem_rep(char *start, int n);

void print_all_headers(HEADER *header);

void print_sound_data(HEADER *header);

#define TRUE 1 
#define FALSE 0

#endif
