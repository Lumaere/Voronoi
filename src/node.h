#pragma once

#include "DCEL/DCEL.h"
#include "math/point.h"

struct event; // forward declaration of event

using pnt = point<double>;

struct node {
    // leaf node constructor
    node(pnt p);
    // internal node constructor
    node(node *l, node *r, node *ls, node *rs);

    node* intersection(pnt p, double y);
    node* lpar() const;
    node* rpar() const;
    node* prev() const;
    node* next() const;

    bool isLeaf;
    node *parent;

    /* internal node information */
    node *left;
    node *right;
    node *lsite;
    node *rsite;
    DCEL::half_edge *trace;

    /* leaf node information */
    event *circle;
    point<double> site;
};

