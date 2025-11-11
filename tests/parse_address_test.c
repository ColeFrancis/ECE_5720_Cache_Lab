#include "stdio.h"

void parseAddress(unsigned int address, unsigned int* tag, unsigned int* set_index, unsigned int* block_offset, int num_set_index_bits, int num_block_bits)
{
    unsigned int block_bit_mask = (1U << num_block_bits) - 1;
    unsigned int set_bits_mask = (1U << num_set_index_bits) - 1;

    *block_offset = address & block_bit_mask;

    address >>= num_block_bits;
    *set_index = address & set_bits_mask;

    address >>= num_set_index_bits;
    *tag = address;
}

int main(void) 
{
    unsigned int addr = 0x1677;
    unsigned int tag;
    unsigned int set_index;
    unsigned int block_offset;

    parseAddress(addr, &tag, &set_index, &block_offset, 8, 3);

    printf("%x, %x, %x, %x\n", addr, tag, set_index, block_offset);

    return 0;
}