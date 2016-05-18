#include "printer.h"
#include "wav_io.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    HEADER header;

    //Initialize in 0
    memset(&header, 0, sizeof(header));

    FILE *ptr;
    if (argc < 2){
        printf("No wav provided.\n");
        exit(1);
    }
    char *filename = get_file_path(argv[1]);
    if (filename==NULL) {
        exit(1);
    }

    // open file
    ptr = fopen(filename, "r");
    if (ptr == NULL) {
       printf("Error opening file\n");
       exit(1);
    }

    read_headers(&header,ptr);

    print_all_headers(&header);
    print_extra_data(&header.header_p);

     // cleanup before quitting

    filename = get_file_path("samples/dup.wav");

    if (filename==NULL) {
        exit(1);
    }

    FILE *ptr_write = fopen(filename, "w");

    write_headers(&header, ptr_write);
    write_sound_data(&header, ptr_write);

    /* fclose(ptr); */
    free(filename);

    return 0;

}
