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

Test(instructions_set, one_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x1666;
    uint16_t address = 0x0666;
    one_case(chip, opcode);
    cr_expect_eq(chip->PC, address);
    free_chip8(chip);
}

Test(instructions_set, two_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x2895;
    uint16_t address = 0x0895;
    uint16_t pc = chip->PC;
    two_case(chip,  opcode);
    cr_expect_eq(chip->PC, address);
    cr_expect_eq(chip->stack[chip->SP], pc);
    free_chip8(chip);
}

Test(instructions_set, three_case_true)
{
    chip8 *chip = init_chip8();
    uint16_t val = 0x0042;
    uint16_t opcode = 0x3542;
    uint16_t pc = chip->PC + 4;
    chip->V[5] = val;
    three_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, three_case_false)
{
    chip8 *chip = init_chip8();
    uint16_t val = 0x0042;
    uint16_t opcode = 0x3524;
    uint16_t pc = chip->PC + 2;
    chip->V[5] = val;
    three_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, four_case_true)
{
    chip8 *chip = init_chip8();
    uint16_t val = 0x0042;
    uint16_t opcode = 0x3524;
    uint16_t pc = chip->PC + 4;
    chip->V[5] = val;
    four_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, four_case_false)
{
    chip8 *chip = init_chip8();
    uint16_t val = 0x0042;
    uint16_t opcode = 0x3542;
    uint16_t pc = chip->PC + 2;
    chip->V[5] = val;
    four_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, five_case_true)
{
    chip8 *chip = init_chip8();
    uint16_t val = 0x0042;
    uint16_t opcode = 0x5520;
    uint16_t pc = chip->PC + 4;
    chip->V[5] = val;
    chip->V[2] = val;
    five_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, five_case_false)
{
    chip8 *chip = init_chip8();
    uint16_t val = 0x0042;
    uint16_t opcode = 0x5520;
    uint16_t pc = chip->PC + 2;
    chip->V[5] = val;
    five_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, six_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x6501
    uint16_t res = 0x0001;
    uint16_t pc = chip->PC + 2;
    six_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0001);
    free_chip8(chip);
}
