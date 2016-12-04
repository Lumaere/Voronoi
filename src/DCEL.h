#pragma once

#include "Point.h"

#include <vector>

/* does not support holes in faces */
struct half_edge;

struct vertex {
    vertex (point<double> p)
        : coordinates{p}
    {}
    half_edge *rep; /* rep->tail == this */
    point<double> coordinates;
};

struct face {
    half_edge *rep; /* rep->left == this */
};
extern std::vector<half_edge*> hold;

struct half_edge {
    half_edge()
        : prev{nullptr}, next{nullptr}, twin{nullptr},
          tail{nullptr}, head{nullptr}, left{nullptr}
    {
        hold.push_back(this);
    }
    /* create half_edge from its twin */
    half_edge(half_edge *twin)
        : half_edge()
    {
        this->twin = twin;
        twin->twin = this;
    }

    half_edge *prev; /* prev->next == this */
    half_edge *next; /* next->prev == this */
    half_edge *twin; /* twin->twin == this */
    vertex *tail;    /* twin->next->tail == tail &&
                        prev->twin->tail == tail */
    vertex *head;
    face *left;      /* prev->left == left && next->left == left */
};

