#include <criterion/criterion.h>

#include "chip8/chip8.h"

Test(chip8_init, chip8_init)
{
    chip8 *chip = init_chip8();
    cr_expect_not_null(chip);
}
