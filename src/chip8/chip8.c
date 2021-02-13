#include "chip8.h"

chip8 *init_chip8(void)
{
    chip8 *chip = malloc(sizeof(chip8)); //init chip8 object
    if (chip == NULL)
        return NULL;

    chip->stack = calloc(16 * sizeof(uint16_t), 1);
    if (chip->stack == NULL)
        return NULL;

    chip->memory = calloc(2048 * sizeof(uint8_t), 1); //init screen
    if (chip->memory == NULL)
    {
        return NULL;
    }

    chip->SP = 0; //set stack pointer
    chip->PC = 0x200; //set program counter
    return chip;
}
/*
void emulate(chip8 *chip)
{
    uint8_t *opcode = chip.memory[chip
}*/
