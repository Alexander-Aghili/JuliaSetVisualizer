#include "helpers.h"

int argparser(int argc, char **argv, int* num_print, int* num_chunks) {
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'p':
            *num_print = atoi(optarg); 
            break;
        case 'c':
            *num_chunks = atoi(optarg); 
            break;
        default:
            break;
        }
    }
    return 0;
}
