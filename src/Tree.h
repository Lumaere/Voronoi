#pragma once

#include "DCEL.h"
#include "Event.h"
#include "Point.h"

#include <iostream>

using pnt = point<double>;

struct node {
    // leaf node constructor
    node(pnt p) 
        : isLeaf{true},
          parent{nullptr},
          left{nullptr},
          right{nullptr},
          lsite{nullptr},
          rsite{nullptr},
          circle{nullptr},
          site{p}
    {} 
    // internal node constructor
    node(node *l, node *r, node *ls, node *rs) 
        : isLeaf{false},
          parent{nullptr},
          left{l},
          right{r},
          lsite{ls},
          rsite{rs}
    {}
          

    node* intersection(pnt p, double y);
    bool below(const pnt &p, double y) const;
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
    half_edge *trace;

    /* leaf node information */
    event *circle;
    point<double> site;
};


class tree {
public:
    tree() : root{nullptr} {}

    node* insert(pnt p, double y);
    node* erase(node *arc, double y);

    /* print beach_line by arcs */
    void print_leaves() const;
    bool empty() const { return root == nullptr; }

    /* do a binary search for the beachline y-value for x-value given by p and
     * current sweepline y */
    bool below(const pnt &p, double y) const
        { return root->below(p, y); }

private:
    void print_tree(node *, int) const;
    void add_endpoints(node *lftB, node *rhtB, node *arc) const;
    node *root;
};

