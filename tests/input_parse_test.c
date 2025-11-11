#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
                return 0;

            i++; // inc i to skip over contents of flag
            s_flag_included = 1;
        }
        else if (strcmp(argv[i], "-E") == 0 && (i+1 < argc)) {
            if ( !(*num_lines_per_set = atoi(argv[i+1])) ) 
                return 0;

            i++; // inc i to skip over contents of flag
            e_flag_included = 1;
        }
        else if (strcmp(argv[i], "-b") == 0 && (i+1 < argc)) {
            if ( !(*num_block_bits = atoi(argv[i+1])) ) 
                return 0;

            i++; // inc i to skip over contents of flag
            b_flag_included = 1;
        }
        else if (strcmp(argv[i], "-t") == 0 && (i+1 < argc)) {
            if ( !(*trace_file_name = argv[i+1]) ) 
                return 0;

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

    printf("%d, %d, %d, %s\n", num_set_index_bits, num_lines_per_set, num_block_bits, trace_file_name);
    return 0;
}