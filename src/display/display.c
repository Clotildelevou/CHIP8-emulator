#include "display.h"

int init_window(SDL_Window **window, SDL_Renderer **renderer)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        fprintf(stderr, "Error SDL_Init : %s", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("chip8-emulator", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 1280, 640, SDL_WINDOW_SHOWN);
    if(*window == NULL)
    {
        fprintf(stderr, "Error SDL_CreateWindow : %s\n", SDL_GetError());
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if(*renderer == NULL)
    {
        fprintf(stderr, "Error SDL_CreateRenderer : %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

static int draw_pixel(SDL_Renderer *renderer, int x, int y)
{
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0)
    {
        fprintf(stderr, "Error SDL_SetRenderDrawColor : %s\n", SDL_GetError());
        return -1;
    }
    SDL_Rect pixel = {x, y, 20, 20};
    SDL_RenderFillRect(renderer, &pixel);
    SDL_RenderPresent(renderer);

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
    {
        fprintf(stderr, "Error SDL_SetRenderDrawColor : %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

int display_chip(chip8 *chip, SDL_Window *window, SDL_Renderer *renderer)
{
    //Choose color
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
    {
        fprintf(stderr, "Error SDL_SetRenderDrawColor : %s\n", SDL_GetError());
        return -1;
    }

    if (SDL_RenderClear(renderer) != 0)
    {
        fprintf(stderr, "Error SDL_SetRenderDrawColor : %s\n", SDL_GetError());
        return -1;
    }

    for (size_t x = 0; x < 64; x++)
    {
        for (size_t y = 0; y < 32; y++)
        {
            size_t px = x + (y * 64);
            if (chip->screen[px] == 1)
            {
                draw_pixel(renderer, x * 20, y * 20);
            }
        }
    }
    SDL_RenderPresent(renderer);
    return 0;
}

void quit(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
