#ifndef SRC_CONFIG_H
#define SRC_CONFIG_H

#define WIN_HEIGHT (int)1000
#define WIN_WIDTH  (int)1200
#define WIN_SIZE   (int)WIN_WIDTH * WIN_HEIGHT
#define MAX_SIZE_INFO_STR (int)100
#define COUNT_ENTITY 10

typedef struct Vect2_t_ {
    int x;
    int y;
} Vect2_t;

typedef struct Entity_t_ {
    Vect2_t pos;
    Vect2_t dir;
} Entity_t;

typedef struct State_t_ {
    Entity_t entities[COUNT_ENTITY];
} State_t;

#endif // SRC_CONFIG_H