#include <stdio.h>

unsigned int getAddressFromLine(char* line);

int main(){

    FILE* trace_file = fopen("../traces/trans.trace", "r");

    if (trace_file == NULL){
        fprintf(stderr, "Error: file not found\n");
        return 0;
    }

    char line[20];
    while (fgets(line, sizeof(line), trace_file) != NULL) {
        if (line[0] != ' ') {
            continue; // skip instruction fetches
        }

        unsigned int address = getAddressFromLine(line);
        printf("%u\n", address);
    }

}

unsigned int getAddressFromLine(char* line){
    char _type;
    unsigned int return_val;
    int _length;

    sscanf(line, " %c %x,%d", &_type, &return_val, &_length);

    return return_val;
}