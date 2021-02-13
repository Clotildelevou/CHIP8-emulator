#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdio.h>

void print_opcode(uint8_t *opcode, size_t index);

void print_buffer(unsigned char *buffer, size_t size);

int read_file(FILE *file, size_t size);

size_t get_size(FILE *file);

int disassemble(char *filename);

#endif
