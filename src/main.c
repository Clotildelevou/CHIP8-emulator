#include <stdio.h>

#include "display/display.h"


int main(void)
{
    //Init chip8
    chip8 *chip = init_chip8();
    if (chip == NULL)
    {
        free_chip8(chip);
        return -1;
    }

    //Graphic part
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int res = init_window(&window, &renderer);
    if (res != 0)
    {
        quit(window, renderer);
        return res;
    }

    SDL_Event event;
    int exit = 0;
    while (!exit)
    {
        res = display_chip(chip, window, renderer);
        //emulate(chip);
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
            exit = SDL_TRUE;
    }

    quit(window, renderer);
    free_chip8(chip);
    return res;
}
