#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

#include "chip8/chip8.h"

void listen_keyboard(chip8 *chip, int *exit, SDL_Event *event);

#endif
