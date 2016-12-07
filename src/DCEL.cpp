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


