#include "chip8.h"

// plural cases for 0
void zero_case(chip8 *chip, uint16_t opcode)
{
    opcode &= 0x000F; // mask the 3 first

    // 0x00E0 CLS (Clear display)
    if (opcode == 0x0000)
    {
        for (size_t i = 0; i < 2048; i++)
        {
            chip->screen[i] = 0;
        }
    }

    // 0x00EE RET (Return from subroutine)
    if (opcode == 0x000E)
    {
        chip->PC = chip->SP--;
    }

    // Error
    else
    {
        fprintf(stderr, "Unknown opcode !");
        exit(1);
    }
    chip->PC += 2;
}

// JP (Jump to location nnn)
void one_case(chip8 *chip, uint16_t opcode)
{
    chip->PC = opcode & 0x0FFF;
}

// Call (call subrout at nnn)
void two_case(chip8 *chip, uint16_t opcode)
{
    chip->SP += 2;
    chip->stack[chip->SP] = chip->PC;
    chip->PC = opcode & 0x0FFF;
}

// 3xkk (Skip next instruction if Vx = kk)
void three_case(chip8 *chip, uint16_t opcode)
{
    uint16_t val = opcode & 0x00FF;

    // shift to turn 0x0X00 → 0x000X
    uint16_t index = opcode & 0x0F00 >> 8;
    if (chip->V[index] == val)
        chip->PC += 4;
    else
        chip->PC += 2;
}

// 4xkk (Skip next instruction if Vx != kk)
void four_case(chip8 *chip, uint16_t opcode)
{
    uint16_t val = opcode & 0x00FF;

    // shift to turn 0x0X00 → 0x000X
    uint16_t index = opcode & 0x0F00 >> 8;
    if (chip->V[index] != val)
        chip->PC += 4;
    else
        chip->PC += 2;
}

// 5xy0 (Skip next instruction if Vx = Vy)
void five_case(chip8 *chip, uint16_t opcode)
{
    // shift to turn 0x0X00 → 0x000X
    uint16_t x_index = opcode & 0x0F00 >> 8;

    // shift to turn 0x00Y0 → 0x000Y
    uint16_t y_index = opcode & 0x00F0 >> 8;
    if (chip->V[x_index] == chip->V[y_index])
        chip->PC += 4;
    else
        chip->PC += 2;
}

// 6xkk (Set Vx = kk)
void six_case(chip8 *chip, uint16_t opcode)
{
    // shift to turn 0x0X00 → 0x000X
    uint16_t index = opcode & 0x0F00 >> 8;

    uint16_t val = opcode & 0x00FF;
    chip->V[index] = val;
}

// 7xkk (Set Vx = Vx + kk)
void seven_case(chip8 *chip, uint16_t opcode)
{
    // shift to turn 0x0X00 → 0x000X
    uint16_t index = opcode & 0x0F00 >> 8;

    uint16_t val = opcode & 0x00FF;
    chip->V[index] += val;
}

// plural cases for 8
// here we got a lot of them so we'll also use a jump table.

void ld(chip8 *chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    uint16_t y = opcode & 0x00F0 >> 4;
    chip->V[x] = chip->V[y];
    chip->PC += 2;
}

void or (chip8 * chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    uint16_t y = opcode & 0x00F0 >> 4;
    chip->V[x] = chip->V[y] | chip->V[x];
    chip->PC += 2;
}

void and (chip8 * chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    uint16_t y = opcode & 0x00F0 >> 4;
    chip->V[x] = chip->V[y] & chip->V[x];
    chip->PC += 2;
}

void xor(chip8 *chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    uint16_t y = opcode & 0x00F0 >> 4;
    chip->V[x] = chip->V[y] ^ chip->V[x];
    chip->PC += 2;
}

void add(chip8 *chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    uint16_t y = opcode & 0x00F0 >> 4;
    chip->V[x] = chip->V[y] + chip->V[x];
    if (chip->V[y] > (0xFF - chip->V[x]))
        chip->V[0xF] = 1;
    else
        chip->V[0xF] = 0;
    chip->PC += 2;
}

void sub(chip8 *chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    uint16_t y = opcode & 0x00F0 >> 4;
    chip->V[x] = chip->V[y] - chip->V[x];
    if (chip->V[y] > (0xFF - chip->V[x]))
        chip->V[0xF] = 0;
    else
        chip->V[0xF] = 1;
    chip->PC += 2;
}

void shr(chip8 *chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    chip->V[0xF] = chip->V[x] & 0x1;
    chip->V[x] >>= 1;
    chip->PC += 2;
}

void subn(chip8 *chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    uint16_t y = opcode & 0x00F0 >> 4;
    if (chip->V[x] < chip->V[y])
        chip->V[0xF] = 0;
    else
        chip->V[0xF] = 1;
    chip->V[x] = chip->V[y] + chip->V[x];
    chip->PC += 2;
}

void shl(chip8 *chip, uint16_t opcode)
{
    uint16_t x = opcode & 0x0F00 >> 8;
    chip->V[0xF] = chip->V[x] >> 7;
    chip->V[x] <<= 1;
    chip->PC += 2;
}

void eight_case(chip8 *chip, uint16_t opcode)
{
    uint16_t index = opcode & 0x000F;
    void (*cases[])(chip8 *, uint16_t) = { ld,  or,  and,  xor, add,
                                           sub, shr, subn, shl };
    // jump table of possibilities
    if (index == 0xE)
        cases[8](chip, opcode);
    // the 8th instruction is 8xyE
    if (index <= 7)
        cases[index](chip, opcode);
    else
    {
        fprintf(stderr, "Unknown opcode !");
        exit(1);
    }
}

// 9xy0 (Skip next instruction if Vx != Vy)
void nine_case(chip8 *chip, uint16_t opcode)
{
    uint16_t val1 = chip->V[opcode & 0x0F00 >> 8];
    uint16_t val2 = chip->V[opcode & 0x00F0 >> 4];
    if (val1 != val2)
        chip->PC += 4;
    else
        chip->PC += 2;
}
