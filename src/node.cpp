#include "node.h"
#include "math/math.h"

#include <stdexcept>

// leaf node constructor
node::node(pnt p) 
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
node::node(node *l, node *r, node *ls, node *rs) 
    : isLeaf{false},
      parent{nullptr},
      left{l},
      right{r},
      lsite{ls},
      rsite{rs}
{
    l->parent = this;
    r->parent = this;
}

node* node::intersection(pnt p, double y) {
    if (isLeaf) {
        // one of edge values so should be good if we reach here
        // base case (?)
        return this;
    } else {
        double h = parabola_intersection(lsite->site, rsite->site, y);
        if (h < p.x)
            return right->intersection(p, y);
        else
            return left->intersection(p, y);
    }
}

node* node::lpar() const {
    if (!isLeaf)
        throw std::runtime_error("Only valid for leaf nodes");
    node *cur = parent;
    while (cur != nullptr) {
        if (cur->rsite == this) return cur;
        cur = cur->parent;
    }
    return nullptr;
}

node* node::rpar() const {
    if (!isLeaf)
        throw std::runtime_error("Only valid for leaf nodes");
    node *cur = parent;
    while (cur != nullptr) {
        if (cur->lsite == this) return cur;
        cur = cur->parent;
    }
    return nullptr;
}

node* node::prev() const {
    node *tmp = lpar();
    return tmp == nullptr ? nullptr : tmp->lsite;
}

node* node::next() const {
    node *tmp = rpar();
    return tmp == nullptr ? nullptr : tmp->rsite;
}

