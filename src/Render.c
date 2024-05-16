#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "Render.h"

#define M_PI 3.14159265358979323846
#define EPSILON 1e-8
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct RenderThreadInfo_t_ {
    Uint8* pixels;
    Entity_t* entities;
    int count;
} RenderThreadInfo_t;

void* render_in_thread(void* render_thread_info);
static void render_entity(Uint8* pixels, int x1, int y1, int x2, int y2, int cx, int cy, int alpha);
static float area(float x1, float y1, float x2, float y2, float x3, float y3);
static int is_inside_triangle(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

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
        int alpha = entities[i].alpha;
        int x1 = MAX(0, cx - ENTITY_RADIUS - BEAK_LENGTH);
        int y1 = MAX(0, cy - ENTITY_RADIUS - BEAK_LENGTH);
        int x2 = MIN(WIN_WIDTH  - 1, cx + ENTITY_RADIUS + BEAK_LENGTH);
        int y2 = MIN(WIN_HEIGHT - 1, cy + ENTITY_RADIUS + BEAK_LENGTH);

        render_entity(pixels, x1, y1, x2, y2, cx, cy, alpha);
    }

    return NULL;
}

static void render_entity(Uint8* pixels, int x1, int y1, int x2, int y2, int cx, int cy, int alpha)
{
    float angle2pi = (float)alpha / (float)DIR_ALPHA_PRECISION * 2.f * M_PI;
    float alpha_x = cos(angle2pi);
    float alpha_y = sin(angle2pi);
    float beak_x = cx + alpha_x * BEAK_LENGTH;
    float beak_y = cy + alpha_y * BEAK_LENGTH;

    float beak1_x = cx + alpha_y * ENTITY_RADIUS;
    float beak1_y = cy - alpha_x * ENTITY_RADIUS;

    float beak2_x = cx - alpha_y * ENTITY_RADIUS;
    float beak2_y = cy + alpha_x * ENTITY_RADIUS;

    for (int y = y1; y <= y2; y++) {
        int dy = cy - y;
        for (int x = x1; x <= x2; x++) {
            int dx = cx - x;

            if (sqrt((dx * dx) + (dy * dy)) <= ENTITY_RADIUS ||
                is_inside_triangle(beak_x, beak_y, beak1_x, beak1_y, beak2_x, beak2_y, (float)x, (float)y)) {
                
                pixels[(y * 4) * WIN_WIDTH + x * 4 + 0] = 0;
                pixels[(y * 4) * WIN_WIDTH + x * 4 + 1] = 0;
                pixels[(y * 4) * WIN_WIDTH + x * 4 + 2] = 0;
            }
        }

    }
}

static float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return fabs(x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)) / 2.f;
}

static int is_inside_triangle(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    float totalArea = area(x1, y1, x2, y2, x3, y3);
    float area1 = area(x1, y1, x2, y2, x4, y4);
    float area2 = area(x2, y2, x3, y3, x4, y4);
    float area3 = area(x3, y3, x1, y1, x4, y4);

    if(abs(totalArea - (area1 + area2 + area3)) <= EPSILON) return 1;
    else                                                    return 0;
}