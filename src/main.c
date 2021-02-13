#include "chip8/chip8.h"
#include <stdio.h>

int main(void)
{
    chip8 *chip = init_chip8();
    if (chip == NULL)
        printf("Initialisation failed.\n");
    else
        printf("Initialisation is a success !\n");
}
