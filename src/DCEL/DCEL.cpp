#include <DCEL/DCEL.h>

#include <cassert>
#include <iostream>

/*
 * vertex
 */
DCEL::vertex::vertex(point<double> p)
    : coordinates{p}
{}

DCEL::vertex::vertex()
    : coordinates({0,0})
{}

DCEL::vertex* DCEL::vertex_factory(point<double> p)
{
    vertex* ret = new vertex(p);
    vertex_list.push_back(ret);
    return ret;
}

/*
 * face
 */
DCEL::face::face(point<double> p)
    : site{p}
{}

DCEL::face::face()
    : site({0,0})
{}

DCEL::face* DCEL::face_factory(point<double> p)
{
    face* ret = new face(p);
    face_list.push_back(ret);
    face_hash.insert(std::make_pair(p,ret));
    return ret;
}

DCEL::face* DCEL::face_of(point<double> p)
{
    /* throw error if non existing */
    return face_hash.at(p);
}

/*
 * half_edge
 */
DCEL::half_edge::half_edge()
    : prev{nullptr}, 
      next{nullptr}, 
      twin{nullptr},
      tail{nullptr}, 
      left{nullptr}
{}

void DCEL::half_edge::set_prev(half_edge *p)
{
    assert(this->prev == nullptr);
    assert(p->next == nullptr);
    this->prev = p;
    p->next = this;
    this->left = this->prev->left;
}

void DCEL::half_edge::set_next(half_edge *n)
{
    assert(this->next == nullptr);
    assert(this->twin->twin == this);
    this->next = n;
    n->prev = this;
    this->left = this->next->left;
}

DCEL::half_edge* DCEL::half_edge_factory()
{
    DCEL::half_edge *ret = new DCEL::half_edge;
    ret->twin = new DCEL::half_edge;
    ret->twin->twin = ret;
    edge_list.push_back(ret);
    edge_list.push_back(ret->twin);
    return ret;
}

/*
 * DCEL general functions
 */
std::tuple<std::vector<DCEL::half_edge*>,
           std::vector<DCEL::vertex*>,
           std::vector<DCEL::face*>>
DCEL::to_vector() const
{
    return std::make_tuple(edge_list,vertex_list,face_list);
}

// checks edge properties for validity
void DCEL::check_edges() const
{
#ifdef DEBUG
    for (auto e : edge_list) {
        if (e->prev) {
            assert(e->prev->next == e);
            assert(e->prev->left == e->left);
        }
        if (e->next) {
            assert(e->next->prev == e);
            assert(e->next->left == e->left);
        }
        assert(e->twin->twin == e);
        if (e->tail) {
            assert(e->twin->next->tail == e->tail);
            assert(e->prev->twin->tail == e->tail);
        }
    }
#endif
}

// add edge representatives for faces
void DCEL::fix_faces()
{
    for (auto e : edge_list)
        if (e->left->rep == nullptr)
            e->left->rep = e;
}

void DCEL::print_dcel(point<double> lower_left, point<double> upper_right) 
    const
{
    for (auto e : edge_list) {
        if (e->tail == nullptr || e->twin->tail == nullptr) continue;
        double x1 = e->tail->coordinates.x, y1 = e->tail->coordinates.y;
        double x2 = e->twin->tail->coordinates.x, y2 = e->twin->tail->coordinates.y;
        if (x1 < lower_left.x || x2 < lower_left.x || 
                y1 < lower_left.y || y2 < lower_left.y) continue;
        if (x1 > upper_right.x || x2 > upper_right.x || 
                y1 > upper_right.y || y2 > upper_right.y) continue;
        std::cout << e->tail->coordinates.x << ' ' 
            << e->tail->coordinates.y << '\n'
            << e->twin->tail->coordinates.x << ' '
            << e->twin->tail->coordinates.y << "\n\n";
    }
}

DCEL::~DCEL()
{
    for (auto e : edge_list)
        delete e;
    for (auto v : vertex_list)
        delete v;
    for (auto f : face_list)
        delete f;
}

/*
 * helper functions
 */
std::pair<DCEL::half_edge*,DCEL::half_edge*> 
match_face(DCEL::half_edge *l, DCEL::half_edge *r)
{
    DCEL::half_edge *toP, *fromP;
    if (l->left == r->left) {
        toP = l;
        fromP = r;
    } else if (l->twin->left == r->left) {
        toP = l->twin;
        fromP = r;
    } else if (l->left == r->twin->left) {
        toP = l;
        fromP = r->twin;
    } else {
        assert(l->twin->left == r->twin->left);
        toP = l->twin;
        fromP = r->twin;
    }
    assert(toP->left == fromP->left);
    return {toP, fromP};
}

