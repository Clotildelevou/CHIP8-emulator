#include "chip8.h"

chip8 *init_chip8(void)
{
    chip8 *chip = malloc(sizeof(chip8)); // init chip8 object
    if (chip == NULL)
    {
        fprintf(stderr, "Couldn't init display");
        exit(1);
    }

    chip->stack = calloc(16 * sizeof(uint16_t), 1); // init stack
    if (chip->stack == NULL)
    {
        fprintf(stderr, "Couldn't init display");
        exit(1);
    }

    chip->memory = calloc(4096 * sizeof(uint8_t), 1); // init memory
    if (chip->memory == NULL)
    {
        fprintf(stderr, "Couldn't init display");
        exit(1);
        return NULL;
    }

    chip->screen = calloc(2048 * sizeof(uint8_t), 1); // init screen
    if (chip->memory == NULL)
    {
        fprintf(stderr, "Couldn't init display");
        exit(1);
    }
    chip->SP = 0; // set stack pointer
    chip->PC = 0x200; // set program counter
    return chip;
}

void emulate(chip8 *chip)
{
    uint16_t opcode = chip->memory[chip->PC] << 8
        | chip->memory[chip->PC + 1]; // fetches the opcode

    void (*cases[])(chip8 *, uint16_t) = { zero_case,  one_case,   two_case,
                                           three_case, four_case,  five_case,
                                           six_case,   seven_case, eight_case,
                                           nine_case };
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
}
