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
    uint16_t opcode = 0x6501;
    uint16_t res = 0x0001;
    uint16_t pc = chip->PC + 2;
    six_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], res);
    free_chip8(chip);
}

Test(instructions_set, seven_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x7501;
    chip->V[5] = 0x0001;
    uint16_t res = 0x0002;
    uint16_t pc = chip->PC + 2;
    seven_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], res);
    free_chip8(chip);
}

Test(instructions_set, eight_ld_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8540;
    chip->V[4] = 0x0001;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], chip->V[4]);
    free_chip8(chip);
}

Test(instructions_set, eight_or_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8541;
    chip->V[4] = 0x0001;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], chip->V[4]);
    free_chip8(chip);
}

Test(instructions_set, eight_and_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8542;
    chip->V[4] = 0x0001;
    chip->V[5] = 0x0000;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0000);
    free_chip8(chip);
}

Test(instructions_set, eight_xor_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8543;
    chip->V[4] = 0x0001;
    chip->V[5] = 0x0000;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0001);
    free_chip8(chip);
}

Test(instructions_set, eight_add_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8544;
    chip->V[4] = 0x0008;
    chip->V[5] = 0x0002;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x000A);
    cr_expect_eq(chip->V[0xF], 0x0000);
    free_chip8(chip);
}

Test(instructions_set, eight_add_case_carry)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8544;
    chip->V[4] = 0x00FF;
    chip->V[5] = 0x00FF;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x00FE);
    cr_expect_eq(chip->V[0xF], 0x0001);
    free_chip8(chip);
}

Test(instructions_set, eight_sub_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8545;
    chip->V[4] = 0x00FF;
    chip->V[5] = 0x000F;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x00F0);
    cr_expect_eq(chip->V[0xF], 0x0000);
    free_chip8(chip);
}

Test(instructions_set, eight_sub_case_borrow)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8545;
    chip->V[4] = 0x000F;
    chip->V[5] = 0x00FF;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0010);
    cr_expect_eq(chip->V[0xF], 0x00001);
    free_chip8(chip);
}

Test(instructions_set, eight_shr_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8546;
    chip->V[5] = 0x0001;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0000);
    cr_expect_eq(chip->V[0xF], 0x00001);
    free_chip8(chip);
}

Test(instructions_set, eight_shr_case_no_last)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8546;
    chip->V[5] = 0x0002;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0001);
    cr_expect_eq(chip->V[0xF], 0x00000);
    free_chip8(chip);
}

Test(instructions_set, eight_subn_case_borrow)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8547;
    chip->V[5] = 0x00FF;
    chip->V[4] = 0x000F;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0010);
    cr_expect_eq(chip->V[0xF], 0x0001);
    free_chip8(chip);

}

Test(instructions_set, eight_subn_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8547;
    chip->V[4] = 0x00FF;
    chip->V[5] = 0x000F;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x00F0);
    cr_expect_eq(chip->V[0xF], 0x0000);
    free_chip8(chip);

}

Test(instructions_set, eight_shl_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8548;
    chip->V[5] = 0x00F0;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x00E0);
    cr_expect_eq(chip->V[0xF], 0x00001);
    free_chip8(chip);
}

Test(instructions_set, eight_shl_case_no_first)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x8548;
    chip->V[5] = 0x0001;
    uint16_t pc = chip->PC + 2;
    eight_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->V[5], 0x0002);
    cr_expect_eq(chip->V[0xF], 0x00000);
    free_chip8(chip);
}

Test(instructions_set, nine_case_no_skip)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x9540;
    chip->V[5] = 0x0001;
    chip->V[4] = 0x0001;
    uint16_t pc = chip->PC + 2;
    nine_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, nine_case_skip)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0x9540;
    chip->V[5] = 0x0001;
    chip->V[4] = 0x0002;
    uint16_t pc = chip->PC + 4;
    nine_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, a_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xA123;
    uint16_t pc = chip->PC + 2;
    a_case(chip, opcode);
    cr_expect_eq(chip->I, 0x0123);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, b_case)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xB120;
    chip->V[0x0] = 0x0003;
    uint16_t pc = 0x0123;
    b_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, skp_true)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xE29E;
    chip->V[2] = 0x0003;
    chip->key_flags[0x0003] = 0x0005;
    uint16_t pc = chip->PC + 4;
    e_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}


Test(instructions_set, skp_false)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xE19E;
    chip->V[0x1] = 0x0002;
    uint16_t pc = chip->PC + 2;
    e_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, skpn_true)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xE1A1;
    chip->V[0x1] = 0x0002;
    uint16_t pc = chip->PC + 4;
    e_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, skpn_false)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xE2A1;
    chip->V[2] = 0x0003;
    chip->key_flags[0x0003] = 0x0005;
    uint16_t pc = chip->PC + 2;
    e_case(chip, opcode);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, ld_dt)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xF207;
    chip->delay_timer = 0x1;
    uint16_t pc = chip->PC + 2;
    f_case(chip, opcode);
    cr_expect_eq(chip->V[2], 0x1);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, ld_k_pressed)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xF20A;
    chip->key_wait = 1;
    chip->key_flags[4] = 1;
    uint16_t pc = chip->PC + 2;
    f_case(chip, opcode);
    cr_expect_eq(chip->V[2], 4);
    cr_expect_eq(chip->PC, pc);
    cr_expect_eq(chip->key_wait, 0);
    free_chip8(chip);
}

Test(instructions_set, ld_k_not_pressed)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xF20A;
    chip->key_wait = 1;
    f_case(chip, opcode);
    for (size_t i = 0; i < 16; i++)
    {
        cr_expect_eq(chip->saved_keys[i], chip->saved_keys[i]);
    }
    cr_expect_eq(chip->key_wait, 1);
    free_chip8(chip);
}

Test(instructions_set, ld_k_no_key)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xF20A;
    chip->key_wait = 0;
    f_case(chip, opcode);
    for (size_t i = 0; i < 16; i++)
    {
        cr_expect_eq(chip->saved_keys[i], chip->key_flags[i]);
    }
    cr_expect_eq(chip->key_wait, 1);
    free_chip8(chip);
}

Test(instructions_set, ld_dt2)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xF215;
    chip->V[2] = 0x1;
    uint16_t pc = chip->PC + 2;
    f_case(chip, opcode);
    cr_expect_eq(chip->delay_timer, 0x1);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}

Test(instructions_set, ld_st)
{
    chip8 *chip = init_chip8();
    uint16_t opcode = 0xF218;
    chip->V[2] = 0x1;
    uint16_t pc = chip->PC + 2;
    f_case(chip, opcode);
    cr_expect_eq(chip->sound_timer, 0x1);
    cr_expect_eq(chip->PC, pc);
    free_chip8(chip);
}
