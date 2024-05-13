#ifndef SRC_WINDOW_H
#define SRC_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Config.h"

void window_prepare (SDL_Window** window, SDL_Texture** texture, SDL_Renderer** renderer,
                     Uint8** pixels, TTF_Font** font, State_t* state);

void window_cycle  (SDL_Window** window, SDL_Texture** texture, SDL_Renderer** renderer,
                     Uint8** pixels, TTF_Font** font, State_t* state);

void window_delete (SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,
                    Uint8* pixels, TTF_Font* font);

#endif // SRC_WINDOW_H