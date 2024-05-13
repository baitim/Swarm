#include <stdio.h>
#include <time.h>

#include "ANSI_colors.h"
#include "Calculating.h"
#include "Config.h"
#include "Render.h"
#include "Window.h"

int main()
{
    printf(print_lblue("# Implementation of swarm.\n"
                       "# (c) Baidiusenov Timur, 2024\n\n"));

    srand((unsigned int)time(NULL));

    SDL_Window* window      = NULL;
    SDL_Texture* texture    = NULL;
    SDL_Renderer* renderer  = NULL;
    Uint8* pixels           = NULL;
    TTF_Font* font          = NULL;
    State_t state = {};

    entities_generate(state.entities);

    window_prepare(&window, &texture, &renderer, &pixels, &font, &state);

    window_cycle(&window, &texture, &renderer, &pixels, &font, &state);

    window_delete(window, renderer, texture, pixels, font);

    printf(print_lblue("\n# Bye!\n"));
}