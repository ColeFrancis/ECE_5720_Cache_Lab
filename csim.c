/*
 * Cole Francis (A02365322)
 * Justin Hulme (A02427854)
 * 
 * Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>
 *  -h: Optional help flag that prints usage info
 *  -v: Optional verbose flag that displays trace info
 *  -s <s>: Number of set index bits (S = 2s is the number of sets)
 *  -E <E>: Associativity (number of lines per set)
 *  -b <b>: Number of block bits (B = 2b is the block size)
 *  -t <tracefile>: Name of the valgrind trace to replay
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cachelab.h"

int checkArgs(int argc, char** argv, int* num_set_index_bits, int* num_lines_per_set, int* num_block_bits, char* trace_file_name);

int main(int argc, char **argv)
{
    //if (!checkArgs(argc, argv)) return 0;

    printSummary(0, 0, 0);
    return 0;
}

/**
 * Return 1 if valid, 0 if not
 */
int checkArgs(int argc, char** argv, int* num_set_index_bits, int* num_lines_per_set, int* num_block_bits, char* trace_file_name)
{
    int hFlag = 0;
    int s_flag_included = 0;
    int e_flag_included = 0;
    int b_flag_included = 0; 
    int t_flag_included = 0;

    // Iterate through argv
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h")) {
            hFlag = 1;
        }
        else if (strcmp(argv[i], "-s")) {
            s_flag_included = 1;
            *num_block_bits = atoi(argv[i++]); // inc i to skip over contents of flag
        }
        else if (strcmp(argv[i], "-E")) {
            e_flag_included = 1;
            *num_lines_per_set = atoi(argv[i++]); // inc i to skip over contents of flag
        }
        else if (strcmp(argv[i], "-b")) {
            b_flag_included = 1;
            *num_block_bits = atoi(argv[i++]); // inc i to skip over contents of flag
        }
        else if (strcmp(argv[i], "-t")) {
            t_flag_included = 1;
            trace_file_name = argv[i++];
        }
    }

    if (hFlag){
        printf("Usage: ./csim-ref [-h] -s <s> -E <E> -b <b> -t <tracefile>\n"
               "    -h: Optional help flag that prints usage info\n"
               "    -v: Optional verbose flag that displays trace info\n"
               "    -s <s>: Number of set index bits (S = 2s is the number of sets)\n"
               "    -E <E>: Associativity (number of lines per set)\n"
               "    -b <b>: Number of block bits (B = 2b is the block size)\n"
               "    -t <tracefile>: Name of the valgrind trace to replay");
    }

    if (!(s_flag_included && e_flag_included && b_flag_included && t_flag_included)) {
        fprintf(stderr, "Error: please include all required flags. use -h flag for usage");
        return 0;
    }

    return 1;
}
