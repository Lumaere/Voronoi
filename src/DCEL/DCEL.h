#pragma once

#include "../math/point.h"

#include <vector>
#include <map>
#include <tuple>
#include <cassert>
#include <utility>

class DCEL {
public:
    struct face;
    struct half_edge;
    struct vertex;
    // create half_edge with its twin
    // half_edge's always have a counterpart in our graphs
    half_edge* half_edge_factory();

    // create vertex
    vertex* vertex_factory(point<double> p);

    // create face
    face* face_factory(point<double> p);

    // returns face of input site -> assumes one exists
    face* face_of(point<double> p);

    // returns tuple of vectors for half_edges, vertexes, and faces
    std::tuple<std::vector<half_edge*>,std::vector<vertex*>,std::vector<face*>>
    to_vector() const;

    // checks edge properties for validity
    void check_edges() const;

    // add edge representatives for faces
    void fix_faces();

    // custom destructor to free edges
    ~DCEL();

    struct vertex {
        half_edge *rep; /* rep->tail == this */
        point<double> coordinates;
    private:
        vertex();
        vertex(point<double> p);
        friend vertex* DCEL::vertex_factory(point<double> p);
    };

    /* does not support holes in faces */
    struct face {
        half_edge *rep;     /* rep->left == this */
        point<double> site;
    private:
        face();
        face(point<double> p);
        friend face* DCEL::face_factory(point<double> p);
    };

    // create half_edge with factory
    struct half_edge {
        // sets prev edge for this - assume 'n' has geq information than this
        void set_prev(half_edge *p);
        // similar to set_prev
        void set_next(half_edge *n);

        half_edge *prev; /* prev->next == this */
        half_edge *next; /* next->prev == this */
        half_edge *twin; /* twin->twin == this */
        vertex *tail;    /* twin->next->tail == tail &&
                            prev->twin->tail == tail */
        face *left;      /* prev->left == left && next->left == left */
    private:
        half_edge();
        friend half_edge* DCEL::half_edge_factory();
    };

private:
    std::vector<half_edge*> edge_list;
    std::vector<vertex*> vertex_list;
    std::vector<face*> face_list;
    std::map<point<double>,face*> face_hash;
};


// input: two half_edges with either themselves or twins pointing to a common
//        face
// returns: pair of half_edges that point to common face
std::pair<DCEL::half_edge*,DCEL::half_edge*> 
match_face(DCEL::half_edge *l, DCEL::half_edge *r);

