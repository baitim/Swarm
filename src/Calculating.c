#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Calculating.h"

#define M_PI 3.14159265358979323846

static void entities_convergence(Entity_t* entities);
static void entities_turn       (Entity_t* entities);
static void entities_move       (Entity_t* entities);
static int  check_dist2others   (Entity_t* entities, int ind, float x1, float y1);

void entities_generate(Entity_t* entities)
{
    for (int i = 0; i < COUNT_ENTITY; i++) {
        int next = 1;

        while (next) {
            next = 0;

            float x1 = entities[i].pos.x = rand() % WIN_WIDTH;
            float y1 = entities[i].pos.y = rand() % WIN_HEIGHT;
            float alpha = entities[i].alpha = rand() % DIR_ALPHA_PRECISION;

            for (int j = 0; j < i; j++) {
                float x2 = entities[j].pos.x;
                float y2 = entities[j].pos.y;

                float dx = x2 - x1;
                float dy = y2 - y1;

                float dist = sqrt(dx * dx + dy * dy);

                if (dist < ENTITY_BORDER) {
                    next = 1;
                    break;
                }
            }
        }

        entities[i].active = 1;
    }
}

void update_state(Entity_t* entities)
{
    entities_convergence(entities);
    entities_turn(entities);
    entities_move(entities);
}

static void entities_convergence(Entity_t* entities)
{
    Vect2_t entities_dpos[COUNT_ENTITY] = {};

    for (int i = 0; i < COUNT_ENTITY; i++) {
        for (int j = 0; j < COUNT_ENTITY; j++) {
            if (i == j) continue;

            float dx = entities[j].pos.x - entities[i].pos.x;
            float dy = entities[j].pos.y - entities[i].pos.y;
            float dist_coef = sqrt(dx * dx + dy * dy) / CONVERGENCE_DISTANCE;

            if (dist_coef <= 1.f) {
                entities_dpos[i].x += dx * CONVERGENCE_COEF * dist_coef;
                entities_dpos[i].y += dy * CONVERGENCE_COEF * dist_coef;
            }
        }
    }
    
    for (int i = 0; i < COUNT_ENTITY; i++) {
        float x1 = entities[i].pos.x + entities_dpos[i].x;
        float y1 = entities[i].pos.y + entities_dpos[i].y;

        if (check_dist2others(entities, i, x1, y1)) continue;

        entities[i].pos.x = x1;
        entities[i].pos.y = y1;
    }
}

static void entities_turn(Entity_t* entities)
{
    float entities_dalpha[COUNT_ENTITY] = {};

    for (int i = 0; i < COUNT_ENTITY; i++) {
        int count_of_nearby = 0;
        for (int j = 0; j < COUNT_ENTITY; j++) {
            if (i == j) continue;

            float alpha = entities[j].alpha;
            float dx = entities[j].pos.x - entities[i].pos.x;
            float dy = entities[j].pos.y - entities[i].pos.y;
            float dist_coef = sqrt(dx * dx + dy * dy) / CONVERGENCE_DISTANCE;

            if (dist_coef <= 1.f) {
                entities_dalpha[i] += alpha;
                count_of_nearby++;
            }
        }
        entities_dalpha[i] /= count_of_nearby;
    }

    for (int i = 0; i < COUNT_ENTITY; i++) {
        float alpha  = entities[i].alpha;
        float dalpha = entities_dalpha[i];
        entities[i].alpha = alpha * (1.f - TURN_COEF) + dalpha * TURN_COEF;
    }
}

static void entities_move(Entity_t* entities)
{
    for (int i = 0; i < COUNT_ENTITY; i++) {
        float alpha = ((int)entities[i].alpha % DIR_ALPHA_PER_CIRCLES) / (float)DIR_ALPHA_PER_CIRCLES;
        
        float angle2pi = alpha * 2.f * M_PI;
        float alpha_x = cos(angle2pi);
        float alpha_y = sin(angle2pi);

        float x1 = entities[i].pos.x + (float)SPEED * alpha_x;
        float y1 = entities[i].pos.y + (float)SPEED * alpha_y;

        if (check_dist2others(entities, i, x1, y1)) continue;

        entities[i].pos.x = x1;
        entities[i].pos.y = y1;
    }
}

static int check_dist2others(Entity_t* entities, int ind, float x1, float y1)
{
    for (int i = 0; i < COUNT_ENTITY; i++) {
        if (i == ind) continue;

        float x2 = entities[i].pos.x;
        float y2 = entities[i].pos.y;

        float dx = x2 - x1;
        float dy = y2 - y1;

        float dist = sqrt(dx * dx + dy * dy);

        if (dist < ENTITY_BORDER)
            return 1;
    }
    return 0;
}