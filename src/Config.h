#ifndef SRC_CONFIG_H
#define SRC_CONFIG_H

#define WIN_HEIGHT (int)1000
#define WIN_WIDTH  (int)1200
#define WIN_SIZE   (int)WIN_WIDTH * WIN_HEIGHT
#define PIX_SIZE   (int)WIN_SIZE * 4
#define MAX_SIZE_INFO_STR       (int)100
#define COUNT_ENTITY            (int)100
#define COUNT_RENDER_THREADS    (int)3
#define DIR_ALPHA_PRECISION     (int)10000
#define ENTITY_RADIUS   (int)8
#define BEAK_LENGTH     (int)25
#define SPEED           (int)1.f

typedef struct Vect2_t_ {
    float x;
    float y;
} Vect2_t;

typedef struct Entity_t_ {
    Vect2_t pos;
    float alpha;
} Entity_t;

typedef struct State_t_ {
    Entity_t entities[COUNT_ENTITY];
    unsigned char background[PIX_SIZE];
} State_t;

#endif // SRC_CONFIG_H