#include <time.h>

#include "Render.h"

void render_state(Uint8* pixels, State_t* state)
{
    for (int i = 0; i < WIN_SIZE * 4; i++)
        pixels[i] = rand() % 255;
}