#include "arguments.h"
#include <getopt.h>
#include <openssl/opensslv.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void usage() {
    printf("%s",
           "Usage: stegowav [arguments]\n\n"
           "Arguments:\n"
           "  -e, --embed    embed mode\n"
           "  -x, --extract  extract mode\n"
           "  -i, --in       input fide\n"
           "  -p             WAVE carrier file\n"
           "  -o, --out      output file\n"
           "  -s, --steg     steganography algorithm [optional]\n"
           "                 <LSB1|LSB4|LSBE>\n"
           "  -a             encryption algorithm [optional]\n"
           "                 <aes128|aes192|aes256|des>\n"
           "  -m             encryption cipher mode [optional]\n"
           "                 <ecb|cfb|ofb|cbc>\n"
           "  --pass         encryption password [optional]\n");
}

void version() {
    printf("stegowav version %s\n", STEGOWAV_VERSION_TEXT);
    printf("built with %s\n", OPENSSL_VERSION_TEXT);
}

void args_parse(int argc, char** argv, Arguments* arguments) {
    memset(arguments, 0, sizeof(*arguments));
    int c;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},       {"version", no_argument, 0, 'v'},
        {"embed", no_argument, 0, 'e'},      {"extract", no_argument, 0, 'x'},
        {"in", required_argument, 0, 'i'},   {"p", required_argument, 0, 'p'},
        {"out", required_argument, 0, 'o'},  {"steg", required_argument, 0, 's'},
        {"alg", required_argument, 0, 'a'},  {"mod", required_argument, 0, 'm'},
        {"pass", required_argument, 0, 'w'}, {0, 0, 0, 0}};
    /* getopt_long stores the option index here. */
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "hvexi:p:o:s:a:m:w:", long_options, &option_index)) !=
           -1) {
        switch (c) {
            case 'h':
                usage();
                exit(0);
                break;
            case 'v':
                version();
                exit(0);
                break;
            case 'e':
                /* puts ("option -e or --embed"); */
                if (arguments->mode == EXTRACT) {
                    fprintf(stderr, "ERROR - can't extract and embed at the same time \n");
                    exit(1);
                }
                arguments->mode = EMBED;
                break;
            case 'x':
                /* puts("option -x or --extract"); */
                if (arguments->mode == EMBED) {
                    fprintf(stderr, "ERROR - can't extract and embed at the same time \n");
                    exit(1);
                }
                arguments->mode = EXTRACT;
                break;
            case 'i':
                /* printf("option -i  or -- in with value `%s'\n", optarg); */
                arguments->in_file = optarg;
                break;
            case 'p':
                /* printf("option -p or --p with value `%s'\n", optarg); */
                arguments->p_wavefile = optarg;
                break;
            case 'o':
                /* printf("option -o or --out with value `%s'\n", optarg); */
                arguments->out_file = optarg;
                break;
            case 's':
                /* printf("option -s or --steg with value `%s'\n", optarg); */
                if (strlen(optarg) != 4) {
                    fprintf(stderr,
                            "ERROR - wrong argument for --steg. Options are: "
                            "<LSB1|LSB4|LSBE> \n");
                    exit(1);
                }
                if (strcmp(optarg, "LSB1") == 0) {
                    arguments->steg = LSB1;
                } else if (strcmp(optarg, "LSB4") == 0) {
                    arguments->steg = LSB4;
                } else if (strcmp(optarg, "LSBE") == 0) {
                    arguments->steg = LSBE;
                } else {
                    fprintf(stderr,
                            "ERROR - wrong argument for --steg. Options are: "
                            "<LSB1|LSB4|LSBE> \n");
                    exit(1);
                }
                break;
            case 'a':
                /* printf("option -a or -a with value `%s'\n", optarg); */
                if (strlen(optarg) != 6 && strlen(optarg) != 3) {
                    fprintf(stderr,
                            "a ERROR - wrong argument for --a. Options are: "
                            "<aes128|aes192|aes256|des> %lu\n",
                            strlen(optarg));
                    exit(1);
                }
                if (strcmp(optarg, "aes128") == 0) {
                    arguments->encryption.algorithm = AES128;
                } else if (strcmp(optarg, "aes192") == 0) {
                    arguments->encryption.algorithm = AES192;
                } else if (strcmp(optarg, "aes256") == 0) {
                    arguments->encryption.algorithm = AES256;
                } else if (strcmp(optarg, "des") == 0) {
                    arguments->encryption.algorithm = DES;
                } else {
                    fprintf(stderr,
                            "b ERROR - wrong argument for --a. Options are: "
                            "<aes128|aes192|aes256|des> %lu\n",
                            strlen(optarg));
                    exit(1);
                }
                break;
            case 'm':
                /* printf("option -m or --m with value `%s'\n", optarg); */
                if (strlen(optarg) != 3) {
                    fprintf(stderr,
                            "ERROR - wrong argument for --m. Options are: "
                            "<ecb|cfb|ofb|cbc> \n");
                    exit(1);
                }
                if (strcmp(optarg, "ecb") == 0) {
                    arguments->encryption.mode = ECB;
                } else if (strcmp(optarg, "cfb") == 0) {
                    arguments->encryption.mode = CFB;
                } else if (strcmp(optarg, "ofb") == 0) {
                    arguments->encryption.mode = OFB;
                } else if (strcmp(optarg, "cbc") == 0) {
                    arguments->encryption.mode = CBC;
                } else {
                    fprintf(stderr,
                            "ERROR - wrong argument for --m. Options are: "
                            "<ecb|cfb|ofb|cbc> \n");
                    exit(1);
                }
                break;

            case 'w':
                /* printf("option -w or --pass with value `%s'\n", optarg); */
                arguments->encryption.password = optarg;

                // Default values
                if (arguments->encryption.mode == 0) {
                    arguments->encryption.mode = CBC;
                }
                if (arguments->encryption.algorithm == 0) {
                    arguments->encryption.algorithm = AES128;
                }
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                printf("DEFAULT with value `%c'\n", c);
                exit(1);
        }
    }
    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        fprintf(stderr, "ERROR - unrecognized arguments: ");
        while (optind < argc) printf("%s ", argv[optind++]);
        putchar('\n');
        exit(1);
    }

    if (arguments->encryption.password == NULL &&
        (arguments->encryption.mode != M_NONE || arguments->encryption.algorithm != A_NONE)) {
        fprintf(stderr, "ERROR - must send a password when indicating mode or algorithm\n");
        exit(1);
    }
}

void args_print(Arguments* arguments) {
    printf("MODE %u\n", arguments->mode);
    printf("IN %s\n", arguments->in_file);
    printf("P_WAVEFILE  %s\n", arguments->p_wavefile);
    printf("OUT %s\n", arguments->out_file);
    printf("STEG %u\n", arguments->steg);
    printf("A %u\n", arguments->encryption.algorithm);
    printf("M %u\n", arguments->encryption.mode);
    printf("PASS %s\n", arguments->encryption.password);
}
