#include <criterion/criterion.h>

#include "chip8/chip8.h"

Test(chip8_init, chip8_init)
{
    chip8 *chip = init_chip8();
    cr_expect_not_null(chip);
    free_chip8(chip);
}

Test(instructions_set, zero_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x00E0;
    zero_case(chip, opcode);
    for (size_t i = 0; i < 2048; i++)
        cr_expect_eq(chip->screen[i], 0);

    free_chip8(chip);
}

