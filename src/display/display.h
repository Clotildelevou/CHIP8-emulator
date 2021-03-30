#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#include "chip8/chip8.h"

int init_window(SDL_Window **window, SDL_Renderer **renderer);
int display_chip(chip8 *chip, SDL_Window *window, SDL_Renderer *renderer);
void quit(SDL_Window *window, SDL_Renderer *renderer);


#endif
