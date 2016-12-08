#pragma once

#include "Point.h"

#include <vector>
#include <map>
#include <cassert>
#include <utility>

/* does not support holes in faces */
struct half_edge;
struct vertex;

extern std::vector<vertex*> holdV;
extern std::vector<half_edge*> holdE;

struct vertex {
    vertex (point<double> p)
        : coordinates{p}
    {
        holdV.push_back(this);
    }
    half_edge *rep; /* rep->tail == this */
    point<double> coordinates;
};

struct face {
    face (point<double> p)
        : site{p}
    {
        assert(hash.find(p) == hash.end());
        hash.insert(std::make_pair(p,this));
    }
    half_edge *rep;     /* rep->left == this */
    point<double> site;

    static face* face_of(point<double> p)
    {
        /* throw error if non existing */
        return hash.at(p);
    }
private:
    static std::map<point<double>,face*> hash;
};

// create half_edge with factory
struct half_edge {
    // sets prev edge for this - assume 'n' has geq information than this
    void set_prev(half_edge *p)
    {
        assert(this->prev == nullptr);
        assert(p->next == nullptr);
        this->prev = p;
        p->next = this;
        this->left = this->prev->left;
    }

    // similar to set_prev
    void set_next(half_edge *n)
    {
        assert(this->next == nullptr);
        assert(this->twin->twin == this);
        this->next = n;
        n->prev = this;
        this->left = this->next->left;
    }

    half_edge *prev; /* prev->next == this */
    half_edge *next; /* next->prev == this */
    half_edge *twin; /* twin->twin == this */
    vertex *tail;    /* twin->next->tail == tail &&
                        prev->twin->tail == tail */
    face *left;      /* prev->left == left && next->left == left */
private:
    half_edge()
        : prev{nullptr}, 
          next{nullptr}, 
          twin{nullptr},
          tail{nullptr}, 
          left{nullptr}
    {
        holdE.push_back(this);
    }
    friend half_edge* half_edge_factory();
};

// create half_edge with its twin
// half_edge's always have a counterpart in our graphs
half_edge* half_edge_factory();

// input: two half_edges with either themselves or twins pointing to a common
//        face
// returns: pair of half_edges that point to common face
std::pair<half_edge*,half_edge*> match_face(half_edge *l, half_edge *r);

