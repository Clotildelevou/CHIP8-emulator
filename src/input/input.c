#include "input.h"
#include <string.h>

void listen_keyboard(chip8 *chip, int *exit, SDL_Event *event)
{
    SDL_PollEvent(event);
    if(event->type == SDL_QUIT)
        *exit = 1;
    if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_e:
            chip->key_flags[0] = 1;
            printf("e\n");
            break;
            case SDLK_r:
            chip->key_flags[1] = 1;
            printf("r\n");
            break;
            case SDLK_t:
            printf("t\n");
            chip->key_flags[2] = 1;
            break;
            case SDLK_y:
            chip->key_flags[3] = 1;
            printf("y\n");
            break;
            case SDLK_d:
            chip->key_flags[4] = 1;
            printf("d\n");
            break;
            case SDLK_f:
            chip->key_flags[5] = 1;
            printf("f\n");
            break;
            case SDLK_g:
            chip->key_flags[6] = 1;
            printf("g\n");
            break;
            case SDLK_h:
            chip->key_flags[7] = 1;
            printf("h\n");
            break;
            case SDLK_c:
            chip->key_flags[8] = 1;
            printf("c\n");
            break;
            case SDLK_v:
            chip->key_flags[9] = 1;
            printf("v\n");
            break;
            case SDLK_b:
            chip->key_flags[0xA] = 1;
            printf("b\n");
            break;
            case SDLK_n:
            chip->key_flags[0xB] = 1;
            printf("n\n");
            break;
            case SDLK_u:
            chip->key_flags[0xC] = 1;
            printf("u\n");
            break;
            case SDLK_i:
            chip->key_flags[0xD] = 1;
            printf("i\n");
            break;
            case SDLK_o:
            chip->key_flags[0xE] = 1;
            printf("o\n");
            break;
            case SDLK_p:
            chip->key_flags[0xF] = 1;
            printf("p\n");
            break;
            default:
            break;
        }
    }
}
