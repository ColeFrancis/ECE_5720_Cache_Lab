#ifndef DUMP_CACHE_H
#define DUMP_CACHE_H

#include "stdio.h"

#define USING_DUMP_CACHE 1

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

void dumpCache(Cache_t* cache) 
{
    printf("[tag  valid  use history]\n");
    for (unsigned int set = 0; set < cache->num_sets; set++) {

        for (unsigned int line = 0; line < cache->num_lines_per_set; line++) {
            printf("[%u  %u  %u] ", 
                    cache->sets[set].lines[line].tag, 
                    cache->sets[set].lines[line].valid, 
                    cache->sets[set].use_history[line]);
        }
        printf("\n");
    }
    printf("\n\thit count: %u\n\tmiss count: %u\n\teviction count: %u\n\n", cache->hit_count, cache->miss_count, cache->eviction_count);
}

#endif