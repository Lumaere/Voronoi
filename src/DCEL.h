#pragma once

#include "Point.h"

/* does not support holes in faces */
struct half_edge;

struct vertex {
    half_edge *rep; /* rep->tail == this */
    point<long double> coordinates;
};

struct face {
    half_edge *rep; /* rep->left == this */
};

struct half_edge {
    half_edge *prev; /* prev->next == this */
    half_edge *next; /* next->prev == this */
    half_edge *twin; /* twin->twin == this */
    vertex *tail;    /* twin->next->tail == tail &&
                        prev->twin->tail == tail */
    face *left;      /* prev->left == left && next->left == left */
};

