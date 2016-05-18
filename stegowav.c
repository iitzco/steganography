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
    char *filename = get_file_path(argc, argv);
    if (filename==NULL) {
        exit(1);
    }

    // open file
    ptr = fopen(filename, "rb");
    if (ptr == NULL) {
       printf("Error opening file\n");
       exit(1);
    }

    int ret = read_headers(&header,ptr);

    if (ret < 0){
        printf("Error\n");
        exit(1);
    }

    print_all_headers(&header);
    print_extra_data(&header.header_p);

    fclose(ptr);

     // cleanup before quitting
    free(filename);
    return 0;

}
