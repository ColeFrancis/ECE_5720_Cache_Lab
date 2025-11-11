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
#include <stdint.h>
#include "cachelab.h"

int checkArgs(int argc, char** argv, int* num_set_index_bits, int* num_lines_per_set, int* num_block_bits, char** trace_file_name);
int getAddressFromLine(char* line);
int loadFromMemory(Cache_t* cache, unsigned int address);

typedef struct {
    uint8_t valid;
    uint32_t tag;
} Cache_Line_t;

typedef struct {
    Cache_Line_t* lines;
    uint8_t* use_history;
} Cache_Set_t;

typedef struct {
    Cache_Set_t* sets;
    uint32_t hit_count;
    uint32_t miss_count;
    uint32_t eviction_count;
} Cache_t;

int main(int argc, char **argv)
{
    int num_set_index_bits;
    int num_lines_per_set;
    int num_block_bits;
    char* trace_file_name;

    if (!checkArgs(argc, argv, &num_set_index_bits, &num_lines_per_set, &num_block_bits, &trace_file_name)) 
    {
        fprintf(stderr, "Error: please include all required flags. Use -h flag for usage info.\n");
        return 0;
    }

    uint8_t num_sets = 1u << num_set_index_bits;
    Cache_t cache;

    cache.sets = (Cache_Set_t*) malloc(num_sets * sizeof(Cache_Set_t));
    cache.hit_count = 0;
    cache.miss_count = 0;
    cache.eviction_count = 0;

    for (int set = 0; set < num_sets; set++)
    {
        cache.sets[set].use_history = (uint8_t*) calloc(num_lines_per_set, sizeof(uint8_t));
        cache.sets[set].lines = (Cache_Line_t*) malloc(num_lines_per_set * sizeof(Cache_Line_t));

        for (int line = 0; line < num_lines_per_set; line++)
        {
            cache.sets[set].lines[line].valid = 0;
            cache.sets[set].lines[line].tag = 0;
        }
    }

    FILE* trace_file = fopen(trace_file_name, "r");

    if (trace_file == NULL){
        fprintf(stderr, "Error: file not found\n");
        return 0;
    }

    char* line[20];
    while (fgets(line, sizeof(line), trace_file) != NULL) {
        if (line[0] != ' ') {
            continue; // skip instruction fetches
        }

        int address = getAddressFromLine(line);
        
        loadFromMemory(&cache, address);
    }

    printSummary(cache.hit_count, cache.miss_count, cache.eviction_count);
    return 0;
}

int loadFromMemory(Cache_t* cache, unsigned int address)
{

    return 0;
}

/**
 * Return 1 if valid, 0 if not
 */
int checkArgs(int argc, char** argv, int* num_set_index_bits, int* num_lines_per_set, int* num_block_bits, char** trace_file_name)
{
    int hFlag = 0;
    int s_flag_included = 0;
    int e_flag_included = 0;
    int b_flag_included = 0; 
    int t_flag_included = 0;

    // Iterate through argv
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            hFlag = 1;
        }
        else if (strcmp(argv[i], "-s") == 0 && (i+1 < argc)) {
            if ( !(*num_set_index_bits = atoi(argv[i+1])) ) 
                break;

            i++; // inc i to skip over contents of flag
            s_flag_included = 1;
        }
        else if (strcmp(argv[i], "-E") == 0 && (i+1 < argc)) {
            if ( !(*num_lines_per_set = atoi(argv[i+1])) ) 
                break;

            i++; // inc i to skip over contents of flag
            e_flag_included = 1;
        }
        else if (strcmp(argv[i], "-b") == 0 && (i+1 < argc)) {
            if ( !(*num_block_bits = atoi(argv[i+1])) ) 
                break;

            i++; // inc i to skip over contents of flag
            b_flag_included = 1;
        }
        else if (strcmp(argv[i], "-t") == 0 && (i+1 < argc)) {
            if ( !(*trace_file_name = argv[i+1]) ) 
                break;

            i++; // inc i to skip over contents of flag
            t_flag_included = 1;
        }
    }

    if (hFlag){
        printf("Usage: ./csim-ref [-h] -s <s> -E <E> -b <b> -t <tracefile>\n"
               "    -h: Optional help flag that prints usage info\n"
               "    -v: Optional verbose flag that displays trace info\n"
               "    -s <s>: Number of set index bits (S = 2s is the number of sets)\n"
               "    -E <E>: Associativity (number of lines per set)\n"
               "    -b <b>: Number of block bits (B = 2b is the block size)\n"
               "    -t <tracefile>: Name of the valgrind trace to replay\n");
    }
    
    return s_flag_included && e_flag_included && b_flag_included && t_flag_included;
}

int getAddressFromLine(char* line){
    char _type;
    int return_val;
    int _length;

    sprintf(line, " %c %x,%d", _type, return_val, _length);

    return return_val;
}