#include <stdlib.h>

#include "Calculating.h"

void entities_generate(Entity_t* entities)
{
    for (int i = 0; i < COUNT_ENTITY; i++) {
        entities[i].pos.x = rand() % WIN_WIDTH;
        entities[i].pos.y = rand() % WIN_HEIGHT;
        entities[i].alpha = rand() % DIR_ALPHA_PRECISION;
    }
}