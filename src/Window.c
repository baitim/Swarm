#include <time.h>

#include "Render.h"
#include "Window.h"

static void lock_texture(SDL_Texture** texture, Uint8** pixels);

static void win_print_text(char* text, SDL_Renderer** renderer, TTF_Font* font,
                           int x_, int y_, int w_, int h_);

void add_state_info (SDL_Renderer** renderer, TTF_Font* font, char* fps_string,
                     clock_t clock_begin, clock_t clock_end, double* old_fps);

void window_prepare(SDL_Window** window, SDL_Texture** texture, SDL_Renderer** renderer,
                    Uint8** pixels, TTF_Font** font)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    *window = SDL_CreateWindow("Swarm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_SetTextureBlendMode(*texture, SDL_BLENDMODE_ADD);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
    SDL_RenderSetScale(*renderer, 1, 1);

    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
                                 WIN_WIDTH, WIN_HEIGHT);

    *pixels = calloc(sizeof(Uint8), WIN_SIZE * 4);


    *font = TTF_OpenFont("arial.ttf", 40);
}

void window_cycle(SDL_Window** window, SDL_Texture** texture, SDL_Renderer** renderer,
                  Uint8** pixels, TTF_Font** font, State_t* state)
{
    Uint8* texture_pixels = calloc(sizeof(Uint8), WIN_SIZE * 4);
    char* fps_string = calloc(MAX_SIZE_INFO_STR, sizeof(char));
 
    double old_fps = 0.f;
    clock_t clock_begin = 0, clock_end = 0;
    SDL_Event event = {};
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;
        }
        
        lock_texture(texture, &texture_pixels);
        render_state(*pixels, state);
        memcpy(texture_pixels, *pixels, WIN_SIZE * 4 * sizeof(Uint8));
        SDL_UnlockTexture(*texture);
        SDL_RenderCopy(*renderer, *texture, NULL, NULL);

        clock_end = clock();
        add_state_info(renderer, *font, fps_string, clock_begin, clock_end, &old_fps);
        clock_begin = clock();

        SDL_RenderPresent(*renderer);
    }

finally:
    free(texture_pixels);
    free(fps_string);
}

void window_delete(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, Uint8* pixels,
                   TTF_Font* font)
{
    free(pixels);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();
}

static void lock_texture(SDL_Texture** texture, Uint8** pixels)
{
    int pitch = WIN_SIZE * 4;

    void** void_pixels = (void**) pixels;
    SDL_LockTexture(*texture, NULL, void_pixels, &pitch);
}

static void win_print_text(char* text, SDL_Renderer** renderer, TTF_Font* font,
                           int x_, int y_, int w_, int h_)
{
    SDL_Color color = {255, 0, 17, 255};

    SDL_Surface* info = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(*renderer, info);

    SDL_Rect rect = {.x = x_, .y = y_, .w = w_, .h = h_};
    
    SDL_RenderCopy(*renderer, texture, NULL, &rect);

    SDL_FreeSurface(info);
    SDL_DestroyTexture(texture);
}

void add_state_info(SDL_Renderer** renderer, TTF_Font* font, char* fps_string,
                      clock_t clock_begin, clock_t clock_end, double* old_fps)
{
    double elapsed_ms = (double)(clock_end - clock_begin) / (double)(CLOCKS_PER_SEC / 1000);
    double fps = 1000.f / elapsed_ms;
    *old_fps = fps;
    snprintf(fps_string, MAX_SIZE_INFO_STR, "fps: %.f", *old_fps);

    win_print_text(fps_string, renderer, font, 10, 10, 125, 45);
}