#ifndef CHIP_8
#define CHIP_8

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct chip8
{
    uint8_t V[16]; // 8 bits registers x16
    uint16_t I; // 16 bits Memory register
    uint16_t SP; // Stack pointer (0x000 to 0xFFF)
    uint16_t PC; // Program counter (0x000 to 0xFFF)

    uint8_t delay_timer; // delay timer
    uint8_t sound_timer; // sound timer

    uint16_t *stack; // 16 levels stack
    uint8_t *memory; // 4Kb memory
    uint8_t *screen; // 2048pixel screen (64x32p)
    uint8_t *key_flags; // Keypad
    uint8_t *saved_keys; // save keypad state
    int key_wait;
} chip8;

chip8 *init_chip8(void); // inits the chip8
void emulate(chip8 *chip); // emulate a cycle (fetch opcode, decode,
                           // execute & update timers)
void zero_case(chip8 *chip, uint16_t opcode);
void one_case(chip8 *chip, uint16_t opcode);
void two_case(chip8 *chip, uint16_t opcode);
void three_case(chip8 *chip, uint16_t opcode);
void four_case(chip8 *chip, uint16_t opcode);
void five_case(chip8 *chip, uint16_t opcode);
void six_case(chip8 *chip, uint16_t opcode);
void seven_case(chip8 *chip, uint16_t opcode);
void eight_case(chip8 *chip, uint16_t opcode);
void nine_case(chip8 *chip, uint16_t opcode);
void a_case(chip8 *chip, uint16_t opcode);
void b_case(chip8 *chip, uint16_t opcode);
void c_case(chip8 *chip, uint16_t opcode);
void d_case(chip8 *chip, uint16_t opcode);
void e_case(chip8 *chip, uint16_t opcode);
void f_case(chip8 *chip, uint16_t opcode);

#endif
