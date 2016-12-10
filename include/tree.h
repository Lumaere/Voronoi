#pragma once

#include <DCEL/DCEL.h>
#include <math/point.h>
#include <event.h>
#include <node.h>

using pnt = point<double>;

/* 
 * have not implementated any form of tree balancing 
 *  note: if data is distributed somewhat evenly across the plane, the tree
 *        will likely self-balance to a certain degree
 */
class tree {
public:
    tree(DCEL *diagram) 
        : root{nullptr}, 
          lo{0},
          diagram{diagram}
    {}

    node* insert(pnt p, double y);
    node* erase(node *arc, double y);

    /* print beach_line by arcs */
    void print_leaves() const;
    bool empty() const { return root == nullptr; }

private:
    node *root;
    double lo;

    DCEL* diagram;

    void print_tree(node *, int) const;
    void init_insertion(pnt, double);
    node* degenerate_insertion(node*, node*, pnt, double);
    DCEL::half_edge* add_endpoints(node *lftB, node *rhtB, node *arc);
};

