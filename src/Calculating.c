#include <stdlib.h>
#include <math.h>

#include "Calculating.h"

#define M_PI 3.14159265358979323846

static void entities_move(Entity_t* entities);

void entities_generate(Entity_t* entities)
{
    for (int i = 0; i < COUNT_ENTITY; i++) {
        entities[i].pos.x = rand() % WIN_WIDTH;
        entities[i].pos.y = rand() % WIN_HEIGHT;
        entities[i].alpha = rand() % DIR_ALPHA_PRECISION;
    }
}

void update_state(Entity_t* entities)
{
    entities_move(entities);
}

static void entities_move(Entity_t* entities)
{
    for (int i = 0; i < COUNT_ENTITY; i++) {
        float angle2pi = (float)entities[i].alpha / (float)DIR_ALPHA_PRECISION * 2.f * M_PI;
        float alpha_x = cos(angle2pi);
        float alpha_y = sin(angle2pi);

        entities[i].pos.x += (float)SPEED * alpha_x;
        entities[i].pos.y += (float)SPEED * alpha_y;
    }
}