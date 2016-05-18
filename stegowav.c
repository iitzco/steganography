#include "printer.h"
#include "wav_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    HEADER header;

    //Initialize in 0
    memset(&header, 0, sizeof(header));

    int ret = read_headers(&header,argc, argv);

    if (ret < 0){
        printf("Error\n");
        exit(1);
    }

    print_all_headers(&header);
    print_extra_data(&header.header_p);

    return 0;

}
