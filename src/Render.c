#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "Render.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct RenderThreadInfo_t_ {
    Uint8* pixels;
    Entity_t* entities;
    int count;
} RenderThreadInfo_t;

void* render_in_thread(void* render_thread_info);

void render_state(Uint8* pixels, State_t* state)
{
    memcpy(pixels, state->background, PIX_SIZE * sizeof(Uint8));
    pthread_t pool[COUNT_RENDER_THREADS];

    int count2thread = COUNT_ENTITY / COUNT_RENDER_THREADS;
    RenderThreadInfo_t render_thread_info[COUNT_RENDER_THREADS];

    for (int num_thread = 0; num_thread < COUNT_RENDER_THREADS; num_thread++) {
        render_thread_info[num_thread] = 
            (RenderThreadInfo_t) {.pixels = pixels,
                                  .entities = state->entities + count2thread * num_thread,
                                  .count = count2thread};

        if (num_thread == COUNT_RENDER_THREADS - 1)
            render_thread_info[num_thread].count = COUNT_ENTITY - count2thread * num_thread;

        pthread_create(&pool[num_thread], NULL, render_in_thread, (void*)&render_thread_info[num_thread]);
    }

    for (int num_thread = 0; num_thread < COUNT_RENDER_THREADS; num_thread++)
        pthread_join(pool[num_thread], NULL);
}

void* render_in_thread(void* render_thread_info_)
{
    RenderThreadInfo_t* render_thread_info = (RenderThreadInfo_t*) render_thread_info_;

    Uint8* pixels      = render_thread_info->pixels;
    Entity_t* entities = render_thread_info->entities;
    int count          = render_thread_info->count;

    for (int i = 0; i < count; i++) {
        int cx = entities[i].pos.x;
        int cy = entities[i].pos.y;
        int x1 = MAX(0, cx - ENTITY_RADIUS);
        int y1 = MAX(0, cy - ENTITY_RADIUS);
        int x2 = MIN(WIN_WIDTH  - 1, cx + ENTITY_RADIUS);
        int y2 = MIN(WIN_HEIGHT - 1, cy + ENTITY_RADIUS);

        for (int y = y1; y <= y2; y++) {
            int dy = cy - y;
            for (int x = x1; x <= x2; x++) {
                int dx = cx - x;

                if (sqrt((dx * dx) + (dy * dy)) <= ENTITY_RADIUS) {
                    pixels[(y * 4) * WIN_WIDTH + x * 4 + 0] = 0;
                    pixels[(y * 4) * WIN_WIDTH + x * 4 + 1] = 0;
                    pixels[(y * 4) * WIN_WIDTH + x * 4 + 2] = 0;
                }
            }
        }
    }

    return NULL;
}