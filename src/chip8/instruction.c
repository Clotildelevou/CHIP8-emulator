#include "chip8.h"

// plural cases for 0
void zero_case(chip8 *chip, uint16_t opcode)
{
    uint16_t cls = 0x00E0;
    uint16_t ret = 0x00EE;
    // 0x00E0 CLS (Clear display)
    if (opcode == cls)
    {
        for (size_t i = 0; i < 2048; i++)
        {
            chip->screen[i] = 0;
        }
    }

    // 0x00EE RET (Return from subroutine)
    else if (opcode == ret)
    {
        chip->PC = chip->SP--;
    }

    // Error
    else
    {
        fprintf(stderr, "Unknown opcode %#x !\n", opcode);
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

// 3xkk (Skip next instruction if Vx == kk)
void three_case(chip8 *chip, uint16_t opcode)
{
    uint16_t val = opcode & 0x00FF;

    // shift to turn 0x0X00 → 0x000X
    uint16_t index = (opcode & 0x0F00) >> 8;
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
    uint16_t index = (opcode & 0x0F00) >> 8;
    if (chip->V[index] != val)
        chip->PC += 4;
    else
        chip->PC += 2;
}

// 5xy0 (Skip next instruction if Vx = Vy)
void five_case(chip8 *chip, uint16_t opcode)
{
    // shift to turn 0x0X00 → 0x000X
    uint16_t x_index = (opcode & 0x0F00) >> 8;

    // shift to turn 0x00Y0 → 0x000Y
    uint16_t y_index = (opcode & 0x00F0) >> 8;
    if (chip->V[x_index] == chip->V[y_index])
        chip->PC += 4;
    else
        chip->PC += 2;
}

// 6xkk (Set Vx = kk)
void six_case(chip8 *chip, uint16_t opcode)
{
    // shift to turn 0x0X00 → 0x000X
    uint16_t index = (opcode & 0x0F00) >> 8;

    uint16_t val = opcode & 0x00FF;
    chip->V[index] = val;
}

// 7xkk (Set Vx = Vx + kk)
void seven_case(chip8 *chip, uint16_t opcode)
{
    // shift to turn 0x0X00 → 0x000X
    uint16_t index = (opcode & 0x0F00) >> 8;

    uint16_t val = opcode & 0x00FF;
    chip->V[index] += val;
}

// plural cases for 8
// here we got a lot of them so we'll also use a jump table.

// 8xy0 (Set Vx = Vy)
static void ld(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    chip->V[x] = chip->V[y];
    chip->PC += 2;
}

// 8xy1 (Vx = Vx OR Vy)
static void or (chip8 * chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    chip->V[x] = chip->V[y] | chip->V[x];
    chip->PC += 2;
}

// 8xy2 (Vx = Vx AND Vy)
static void and (chip8 * chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    chip->V[x] = chip->V[y] & chip->V[x];
    chip->PC += 2;
}

// 8xy3 (Vx = Vx XOR Vy)
static void xor(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    chip->V[x] = chip->V[y] ^ chip->V[x];
    chip->PC += 2;
}

//8xy4 (Set Vx = Vx + Vy, set VF = carry)
static void add(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    chip->V[x] = chip->V[y] + chip->V[x];

    if (chip->V[y] > (0xFF - chip->V[x]))
        chip->V[0xF] = 1;
    else
        chip->V[0xF] = 0;
    chip->PC += 2;
}

// 8xy5 (Set Vx = Vx - Vy, set VF = NOT borrow)
static void sub(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    chip->V[x] = chip->V[y] - chip->V[x];
    if (chip->V[y] > (0xFF - chip->V[x]))
        chip->V[0xF] = 0;
    else
        chip->V[0xF] = 1;
    chip->PC += 2;
}

// 8xy6 (Set Vx = Vx shifted right of 1 byte)
static void shr(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    chip->V[0xF] = chip->V[x] & 0x1;
    chip->V[x] >>= 1;
    chip->PC += 2;
}

// 8xy7 (Set Vx = Vy - Vx, set VF = NOT borrow)
static void subn(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    if (chip->V[x] < chip->V[y])
        chip->V[0xF] = 0;
    else
        chip->V[0xF] = 1;
    chip->V[x] = chip->V[y] + chip->V[x];
    chip->PC += 2;
}

// 8xy6 (Set Vx = Vx shifted left of 1 byte)
static void shl(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    chip->V[0xF] = chip->V[x] >> 7;
    chip->V[x] <<= 1;
    chip->PC += 2;
}

// Jump table for the 8nnn case
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
    uint16_t val1 = chip->V[(opcode & 0x0F00) >> 8];
    uint16_t val2 = chip->V[(opcode & 0x00F0) >> 4];
    if (val1 != val2)
        chip->PC += 4;
    else
        chip->PC += 2;
}

// Annn (Set I = NNN)
void a_case(chip8 *chip, uint16_t opcode)
{
    chip->I = opcode & 0x0FFF;
    chip->PC += 2;
}

// Bnnn (Jump to location nnn + V0)
void b_case(chip8 *chip, uint16_t opcode)
{
    chip->PC = (opcode & 0x0FFF) + (chip->V[0x0]);
}

// Cxkk (Set Vx = kk AND random number)
void c_case(chip8 *chip, uint16_t opcode)
{
    uint16_t val = (rand() % (0xFF + 1)) & (opcode & 0x00FF);
    chip->V[(opcode & 0x0F00) >> 8] = val;
    chip->PC += 2;
}

// Dxyn (Displays n-byte sprite starting at I at Vx, Vy)
void d_case(chip8 *chip, uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    chip->V[0xF] = 0;

    for (size_t y_coord = 0xF; y_coord < (opcode & 0x000F); y_coord++)
    {
        uint16_t pixel = chip->memory[chip->I];
        for (int x_coord = 0; x_coord < 8; x_coord++)
        {
            // If the current pixel in the sprite row is on and the pixel
            // at coordinates X,Y on the screen is also on, turn off the
            // pixel and set VF to 1
            if (((pixel & (1 << x_coord)) > 0)
                && ((x + x_coord + (y + y_coord) * 64) == 1))
            {
                chip->V[0xF] = 1;
            }
            // if the current pixel in the sprite row
            // is on and the screen pixel is not, draw the pixel at the
            // X and Y coordinates
            chip->screen[x + x_coord * ((y + y_coord) * 64)] ^= 1;
        }
    }
    chip->PC += 2;
}

// Ex9E (Skip next instruction if key with the value of Vx is pressed)
static void skp(chip8 *chip, uint16_t opcode)
{
    if (chip->key_flags[chip->V[(opcode & 0x0F00) >> 8]] != 0)
    {
        chip->PC += 2;
    }
}

// Ex9E (Skip next instruction if key with the value of Vx is NOT pressed)
static void skpn(chip8 *chip, uint16_t opcode)
{
    if (chip->key_flags[chip->V[(opcode & 0x0F00) >> 8]] == 0)
    {
        chip->PC += 2;
    }
}

// Handle Exnn cases
void e_case(chip8 *chip, uint16_t opcode)
{
    if ((opcode & 0x00F0) == 0x0090)
        skp(chip, opcode);
    if ((opcode & 0x00F0) == 0x00A0)
        skpn(chip, opcode);
    else
    {
        fprintf(stderr, "Unknown opcode !");
        exit(1);
    }
    chip->PC += 2;
}

// Fx07 (Set Vx = delay timer value)
static void ld_dt(chip8 *chip, uint16_t opcode)
{
    chip->V[(opcode & 0x0F00) >> 8] = chip->delay_timer;
    chip->PC += 2;
}

// Fx0A (Wait for a key press, store the value in Vx)
static void ld_k(chip8 *chip, uint16_t opcode)
{
    if (chip->key_wait == 0)
    {
        memcpy(&chip->saved_keys, &chip->key_flags, 8);
        chip->key_wait = 1;
    }
    else
    {
        for (size_t i = 0; i < 16; i++)
        {
            if ((chip->saved_keys[i] == 0) && (chip->key_flags[i] == 1))
            {
                chip->key_wait = 0;
                chip->V[(opcode & 0x0F00) >> 8] = i;
                chip->PC += 2;
            }
            else
                chip->saved_keys[i] = chip->key_flags[i];
        }
    }
}

// Fx15 (Set delay timer = Vx)
static void ld_dt2(chip8 *chip, uint16_t opcode)
{
    chip->delay_timer = chip->V[(opcode & 0x0F00) >> 8];
    chip->PC += 2;
}

// Fx18 (Set sound timer = Vx)
static void ld_st(chip8 *chip, uint16_t opcode)
{
    chip->sound_timer = chip->V[(opcode & 0x0F00) >> 8];
    chip->PC += 2;
}

// Fx1E (Set I = I + Vx)
static void add_i(chip8 *chip, uint16_t opcode)
{
    chip->I += chip->V[(opcode & 0x0F00) >> 8];
    chip->PC += 2;
}

// Fx29 (Set I = location of sprite for digit Vx)
static void ld_f(chip8 *chip, uint16_t opcode)
{
    uint16_t x_index = (opcode & 0x0F00) >> 8;
    chip->I = (chip->V[x_index] * 0x5);
    chip->PC += 2;
}

// Fx33 (Store BCD representation of Vx in memory locations I, I+1, and I+2)
static void ld_b(chip8 *chip, uint16_t opcode)
{
    uint16_t x_index = (opcode & 0x0F00) >> 8;
    chip->memory[chip->I] = chip->V[x_index] / 100;
    chip->memory[chip->I + 1] = (chip->V[x_index] / 10) % 10;
    chip->memory[chip->I + 2] = (chip->V[x_index] % 100) % 10;
    chip->PC += 2;
}

// Fx55 (Store registers V0 through Vx in memory starting at location I)
static void ld_i(chip8 *chip, uint16_t opcode)
{
    uint16_t max = (opcode & 0x0F00) >> 8;
    for (uint16_t i = 0x0; i < max; i++)
        chip->memory[chip->I + i] = chip->V[i];
    chip->PC += 2;
}

// Fx65 (Store registers through Vx from memory starting at location I)
static void ld_i2(chip8 *chip, uint16_t opcode)
{
    uint16_t max = (opcode & 0x0F00) >> 8;
    for (uint16_t i = 0x0; i < max; i++)
        chip->V[i] = chip->memory[chip->I + i];
    chip->PC += 2;
}

// Fxnn case
void f_case(chip8 *chip, uint16_t opcode)
{
    uint16_t id = (opcode & 0x00FF);
    switch (id)
    {
    case 0x0007:
        ld_dt(chip, opcode);
        break;
    case 0x000A:
        ld_k(chip, opcode);
        break;
    case 0x0015:
        ld_dt2(chip, opcode);
        break;
    case 0x0018:
        ld_st(chip, opcode);
        break;
    case 0x0029:
        ld_f(chip, opcode);
        break;
    case 0x001E:
        add_i(chip, opcode);
        break;
    case 0x0033:
        ld_b(chip, opcode);
        break;
    case 0x0055:
        ld_i(chip, opcode);
        break;
    case 0x0065:
        ld_i2(chip, opcode);
        break;
    default:
        fprintf(stderr, "Unkown opcode !\n");
        exit(1);
        break;
    }
}
