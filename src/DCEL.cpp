#include "DCEL.h"

std::vector<vertex*> holdV;
std::vector<half_edge*> holdE;

std::map<point<double>,face*> face::hash;

half_edge* half_edge_factory()
{
    half_edge *ret = new half_edge;
    ret->twin = new half_edge;
    ret->twin->twin = ret;
    return ret;
}

std::pair<half_edge*,half_edge*> match_face(half_edge *l, half_edge *r)
{
    half_edge *toP, *fromP;
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

