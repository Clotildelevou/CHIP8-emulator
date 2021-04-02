#include <stdio.h>

#include "display/display.h"
#include "input/input.h"


int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "Too much args\n");
        return -1;
    }

    //Init chip8
    chip8 *chip = init_chip8();
    if (chip == NULL)
    {
        free_chip8(chip);
        return -1;
    }

    //Load file
    char *filename = argv[1];
    int loaded_data = load_file(chip, filename, 0L);
    int more_data = loaded_data == 4096;

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
        emulate(chip);
        listen_keyboard(chip, &exit, &event);
        if (chip->PC == 4096 && more_data)
        {
            loaded_data = load_file(chip, filename, loaded_data);
            more_data = loaded_data == 4096;
        }
    }

    quit(window, renderer);
    free_chip8(chip);
    return res;
}
