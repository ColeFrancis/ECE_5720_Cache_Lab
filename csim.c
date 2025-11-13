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

// #define DEBUG

#include "dump_cache.h"

#ifndef USING_DUMP_CACHE
typedef enum {
    M,
    L,
    S,
    I
} INST_t;

typedef struct {
    unsigned int valid;
    unsigned int tag;
} Cache_Line_t;

typedef struct {
    Cache_Line_t* lines;

    unsigned int* use_history;
} Cache_Set_t;

typedef struct {
    Cache_Set_t* sets;
    unsigned int num_sets;
    unsigned int num_lines_per_set;

    unsigned int hit_count;
    unsigned int miss_count;
    unsigned int eviction_count;
} Cache_t;
#endif

void initCache(Cache_t* cache, unsigned int num_set_index_bits, unsigned int num_lines_per_set);
unsigned int checkArgs(int argc, char** argv, unsigned int* num_set_index_bits, unsigned int* num_lines_per_set, unsigned int* num_block_bits, char** trace_file_name);
INST_t getAddressFromLine(char* line, unsigned int* address);
void parseAddress(unsigned int address, unsigned int* tag, unsigned int* set_index, unsigned int* block_offset, unsigned int num_set_index_bits, unsigned int num_block_bits);
void loadFromMemory(Cache_t* cache, unsigned int tag, unsigned int set_index);
void storeToMemory(Cache_t* cache, unsigned int tag, unsigned int set_index);
void updateCacheUseHistory(Cache_Set_t* set, unsigned int line_idx, unsigned int num_lines);
unsigned int getLRUIndex(Cache_Set_t* set, unsigned int num_lines_per_set);

int main(int argc, char **argv)
{
    unsigned int num_set_index_bits;
    unsigned int num_lines_per_set;
    unsigned int num_block_bits;
    char* trace_file_name;

    // Read command line arguments and verify
    if (!checkArgs(argc, argv, &num_set_index_bits, &num_lines_per_set, &num_block_bits, &trace_file_name)) 
    {
        fprintf(stderr, "Error: please include all required flags. Use -h flag for usage info.\n");
        return 0;
    }

    // Open file
    FILE* trace_file = fopen(trace_file_name, "r");
    if (trace_file == NULL){
        fprintf(stderr, "Error: file not found\n");
        return 0;
    }

    Cache_t cache;
    initCache(&cache, num_set_index_bits, num_lines_per_set);

    char line[20];
    while (fgets(line, sizeof(line), trace_file) != NULL) {
        if (line[0] != ' ') {
            continue; // skip instruction fetches
        }

        unsigned int address;
         
        INST_t inst_type = getAddressFromLine(line, &address);

        unsigned int tag;
        unsigned int set_index;
        unsigned int block_offset;

        parseAddress(address, &tag, &set_index, &block_offset, num_set_index_bits, num_block_bits);

        char debug[4] = {'M', 'L', 'S', 'I'};

        printf("%c %u %u %u\n", debug[inst_type], tag, set_index, block_offset);

        switch (inst_type)
        {
        case M:
            loadFromMemory(&cache, tag, set_index);
            dumpCache(&cache);
            storeToMemory(&cache, tag, set_index);
            dumpCache(&cache);
            break;

        case L:
            loadFromMemory(&cache, tag, set_index);
            dumpCache(&cache);
            break;

        case S:
            storeToMemory(&cache, tag, set_index);
            dumpCache(&cache);
            break;
        
        default:
            break;
        }
    }

    printSummary((int)cache.hit_count, (int)cache.miss_count, (int)cache.eviction_count);
    return 0;
}

void initCache(Cache_t* cache, unsigned int num_set_index_bits, unsigned int num_lines_per_set)
{
    unsigned int num_sets = 1u << num_set_index_bits;

    cache->sets = (Cache_Set_t*) malloc(num_sets * sizeof(Cache_Set_t));
    cache->num_sets = num_sets;
    cache->num_lines_per_set = num_lines_per_set;

    cache->hit_count = 0;
    cache->miss_count = 0;
    cache->eviction_count = 0;

    for (unsigned int set = 0; set < num_sets; set++)
    {
        cache->sets[set].use_history = (unsigned int*) calloc(num_lines_per_set, sizeof(unsigned int));
        cache->sets[set].lines = (Cache_Line_t*) malloc(num_lines_per_set * sizeof(Cache_Line_t));

        for (unsigned int line = 0; line < num_lines_per_set; line++)
        {
            cache->sets[set].lines[line].valid = 0;
            cache->sets[set].lines[line].tag = 0;
        }
    }
}

unsigned int checkArgs(int argc, char** argv, unsigned int* num_set_index_bits, unsigned int* num_lines_per_set, unsigned int* num_block_bits, char** trace_file_name)
{
    unsigned int hFlag = 0;
    unsigned int s_flag_included = 0;
    unsigned int e_flag_included = 0;
    unsigned int b_flag_included = 0; 
    unsigned int t_flag_included = 0;

    // Iterate through argv
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            hFlag = 1;
        }
        else if (strcmp(argv[i], "-s") == 0 && (i+1 < argc)) {
            if ( !(*num_set_index_bits = (unsigned int)atoi(argv[i+1])) ) 
                break;

            i++; // inc i to skip over contents of flag
            s_flag_included = 1;
        }
        else if (strcmp(argv[i], "-E") == 0 && (i+1 < argc)) {
            if ( !(*num_lines_per_set = (unsigned int)atoi(argv[i+1])) ) 
                break;

            i++; // inc i to skip over contents of flag
            e_flag_included = 1;
        }
        else if (strcmp(argv[i], "-b") == 0 && (i+1 < argc)) {
            if ( !(*num_block_bits = (unsigned int)atoi(argv[i+1])) ) 
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

INST_t getAddressFromLine(char* line, unsigned int* address){

    char type = '\0';
    unsigned int return_val = 0;
    int _ignore;

    sscanf(line, " %c %x %d", &type, &return_val, &_ignore);

    *address = return_val;

    switch (type)
    {
    case 'M':
        return M;

    case 'L':
        return L;

    case 'S':
        return S;
    
    default: 
        return I;
    }
}

// [t bits][s bits][b bits]
void parseAddress(unsigned int address, unsigned int* tag, unsigned int* set_index, unsigned int* block_offset, unsigned int num_set_index_bits, unsigned int num_block_bits)
{
    unsigned int block_bit_mask = (1U << num_block_bits) - 1;
    unsigned int set_bits_mask = (1U << num_set_index_bits) - 1;

    *block_offset = address & block_bit_mask;

    address >>= num_block_bits;
    *set_index = address & set_bits_mask;

    address >>= num_set_index_bits;
    *tag = address;
}

void loadFromMemory(Cache_t* cache, unsigned int tag, unsigned int set_index)
{
    Cache_Set_t set = cache->sets[set_index];

    for (unsigned int line_idx = 0; line_idx < cache->num_lines_per_set; line_idx++) 
    {
        if (set.lines[line_idx].tag == tag && set.lines[line_idx].valid == 1) {
            cache->hit_count++;

            updateCacheUseHistory(&set, line_idx, cache->num_lines_per_set);
            return; // cache hit no need to search rest of cache
        }
    }

    // cache miss
    cache->miss_count++;

    // see if any cache lines are not valid
    for (unsigned int line_idx = 0; line_idx < cache->num_lines_per_set; line_idx++) 
    {
        if (set.lines[line_idx].valid == 0){
            // mark as valid and set the tag
            set.lines[line_idx].valid = 1;
            set.lines[line_idx].tag = tag;

            updateCacheUseHistory(&set, line_idx, cache->num_lines_per_set);

            return; // no need to evict
        }
    }
    
    // cache miss with eviction
    cache->eviction_count++;

    unsigned int LRU = getLRUIndex(&set, cache->num_lines_per_set);

    set.lines[LRU].tag = tag;
    set.lines[LRU].valid = 1;

    updateCacheUseHistory(&set, LRU, cache->num_lines_per_set);
}

void storeToMemory(Cache_t* cache, unsigned int tag, unsigned int set_index)
{
    Cache_Set_t set = cache->sets[set_index];

    for (unsigned int line_idx = 0; line_idx < cache->num_lines_per_set; line_idx++) 
    {
        if (set.lines[line_idx].tag == tag && set.lines[line_idx].valid == 1) {
            cache->hit_count++;
            
            set.lines[line_idx].valid = 1;

            updateCacheUseHistory(&set, line_idx, cache->num_lines_per_set);
            return; // cache hit no need to search rest of cache
        }
    }

    // cache miss
    cache->miss_count++;

    // see if any cache lines are not valid
    for (unsigned int line_idx = 0; line_idx < cache->num_lines_per_set; line_idx++) 
    {
        if (set.lines[line_idx].valid == 0){
            // mark as valid and set the tag
            set.lines[line_idx].valid = 1;
            set.lines[line_idx].tag = tag;

            updateCacheUseHistory(&set, line_idx, cache->num_lines_per_set);

            return; // no need to evict
        }
    }
    
    // cache miss with eviction
    cache->eviction_count++;

    unsigned int LRU = getLRUIndex(&set, cache->num_lines_per_set);

    set.lines[LRU].tag = tag;
    set.lines[LRU].valid = 1;

    updateCacheUseHistory(&set, LRU, cache->num_lines_per_set);
}

void updateCacheUseHistory(Cache_Set_t* set, unsigned int line_idx, unsigned int num_lines)
{
    for (unsigned int i = 0; i < num_lines; i++)
    {
        set->use_history[i]++;
    }

    set->use_history[line_idx] = 0;
}

unsigned int getLRUIndex(Cache_Set_t* set, unsigned int num_lines_per_set){
    unsigned int LRU_idx = 0;

    for (unsigned int i = 0; i < num_lines_per_set; i++){
        LRU_idx = set->use_history[LRU_idx] > set->use_history[i] ? LRU_idx : i;
    }

    return LRU_idx;
}