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

    uint16_t get_first = 0xF000; // mask the 3 last

    switch (opcode & get_first)
    {
    case 0x0000:
        zero_case(chip, opcode);
        break;
    case 0x1000:
        one_case(chip, opcode);
        break;
    case 0x2000:
        two_case(chip, opcode);
        break;
    case 0x3000:
        three_case(chip, opcode);
        break;
    case 0x4000:
        four_case(chip, opcode);
        break;
    case 0x5000:
        five_case(chip, opcode);
        break;
    case 0x6000:
        break;
    case 0x7000:
        break;
    case 0x8000:
        break;
    case 0x9000:
        break;
    case 0xA000:
        break;
    case 0xB000:
        break;
    case 0xC000:
        break;
    case 0xD000:
        break;
    case 0xE000:
        break;
    case 0xF000:
        break;
    default:
        fprintf(stderr, "Unkown opcode !");
        exit(1);
        break;
    }
}
