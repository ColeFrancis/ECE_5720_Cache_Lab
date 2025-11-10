/*
 * Cole Francis (A02365322)
 * Justin Hulme (A________)
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
#include "cachelab.h"

int checkArgs(int argc, char**argv);

int main(int argc, char **argv)
{
    if (!checkArgs(argc, argv)) return 0;

    printSummary(0, 0, 0);
    return 0;
}

/**
 * Return 1 if valid, 0 if not
 */
int checkArgs(int argc, char**argv) 
{
    if (strcmp(argv[1], "-h")) {
        printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
               "    -h: Optional help flag that prints usage info\n"
               "    -v: Optional verbose flag that displays trace info\n"
               "    -s <s>: Number of set index bits (S = 2s is the number of sets)\n"
               "    -E <E>: Associativity (number of lines per set)\n"
               "    -b <b>: Number of block bits (B = 2b is the block size)\n"
               "    -t <tracefile>: Name of the valgrind trace to replay");

        return 0;
    }
    
    if ()
}
