#include "disassemble.h"
#include <stdio.h>

void print_opcode(uint8_t *opcode, size_t index)
{
    printf("%04x %02x %02x ", index, opcode[0], opcode[1]);
    
    //Get the first byte of the opcode
    uint8_t first_b = (code[0]);
}

void print_buffer(unsigned char *buffer, size_t size);

int read_file(FILE *file, size_t size);

size_t get_size(FILE *file);

int disassemble(char *filename);
