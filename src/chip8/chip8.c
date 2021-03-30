#include "chip8.h"

chip8 *init_chip8(void)
{
    //Chip memory allocation
    chip8 *chip = malloc(sizeof(chip8)); // init chip8 object
    if (chip == NULL)
    {
        fprintf(stderr, "Couldn't init chip\n");
        return NULL;
    }

    //Stack allocation
    chip->stack = calloc(16 * sizeof(uint16_t), 1); // init stack
    if (chip->stack == NULL)
    {
        fprintf(stderr, "Couldn't init stack\n");
        return NULL;
    }

    //Memory allocation
    chip->memory = calloc(4096 * sizeof(uint8_t), 1); // init memory
    if (chip->memory == NULL)
    {
        fprintf(stderr, "Couldn't init memory\n");
        return NULL;
    }

    //Screen allocation
    chip->screen = calloc(2048 * sizeof(uint8_t), 1); // init screen
    if (chip->screen == NULL)
    {
        fprintf(stderr, "Couldn't init display\n");
        return NULL;
    }

    //Key_flags allocation
    chip->key_flags = calloc(16 * sizeof(uint8_t), 1);
    if (chip->key_flags == NULL)
    {
        fprintf(stderr, "Couldn't init key_flags\n");
        return NULL;
    }

    //Memory key allocation
    chip->saved_keys = calloc(16 * sizeof(uint8_t), 1);
    if (chip->saved_keys == NULL)
    {
        fprintf(stderr, "Couldn't init saved keys\n");
        return NULL;
    }

    //Set registers
    for (size_t i = 0; i < 16; i++)
    {
        chip->V[i] = 0;
    }

    chip->SP = 0; // set stack pointer
    chip->PC = 0x200; // set program counter
    chip->delay_timer = 0; // set delay timer
    chip->sound_timer = 0; // set sound timer
    chip->key_wait = 0;
    return chip;
}

void free_chip8(chip8 *chip)
{
    free(chip->stack);
    free(chip->memory);
    free(chip->screen);
    free(chip->key_flags);
    free(chip->saved_keys);
    free(chip);
}

int emulate(chip8 *chip)
{
    //TODO load another chunk if pc = 4096
    
    uint16_t opcode = chip->memory[chip->PC] << 8
        | chip->memory[chip->PC + 1]; // fetches the opcode

    if (opcode != 0)
    {
        void (*cases[])(chip8 *,
                        uint16_t) = { zero_case,  one_case,  two_case, three_case,
                                      four_case,  five_case, six_case, seven_case,
                                      eight_case, nine_case, a_case,   b_case,
                                      c_case,     d_case,    e_case,   f_case };
        // jump table to handle cases
        // the opcode can be : 0xYnnn with Y in [0-F]
        // you can read about nnn in src/chip8/instruction.c

        uint16_t index = opcode & 0xF000 >> 12;
        // We can directly use the first nibble of the opcode to call functions
        // of the jump table.
        // For example: your opcode is 0x8xy0
        // 0x8xy0 & 0xF000 = 0x8000
        // 0x8000 >> 12 = 0x8 = 8
        // cases[8] = eight_case
        cases[index](chip, opcode);
        return 0;
    }
    return 0;
}
