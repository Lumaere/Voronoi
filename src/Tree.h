#pragma once

#include "DCEL.h"
#include "Event.h"
#include "Math.h"

struct root;

class tree {
    tree() : root{nullptr} {}

    node* insert(pnt p, double y)
    {
        if (root == nullptr) {
            root = new node(p);
            return;
        }
        // find arc above the site
        node *abv = root->intersection(p, y);

        // divide old arc and insert new one in between
        node *splitL = new node (abv->site);
        node *splitR = new node (abv->site);
        node *nxt = new node (p);

        // bookkeeping and new arcs
        node *rightE = new node (nxt, splitR, nxt, splitR);
        nxt->parent = rightE;
        splitR->parent = rightE;
        rightE->trace = new half_edge;

        node *leftE = new node (splitL, rightE, splitL, nxt);
        splitL->parent = leftE;
        rightE->parent = leftE;
        leftE->trace = new half_edge;

        if (abv->parent != nullptr) {
            leftE->parent = abv->parent;
            if (abv->parent->left == abv) {
                leftE->parent->left = leftE;
            } else {
                leftE->parent->right = leftE;
            }
        } else {
            root = leftE;
        }
        delete abv;
    }

    node* erase(node *arc, double y)
    {
        node *tmp = arc->prev();
        node *lft = arc->lpar();
        node *rht = arc->rpar();
        lft->trace->tail = right->trace->tail = arc->circle->vertex;

        // we can only delete nodes inside the arc
        // TODO: verify this
        assert(lft != nullptr && rht != nullptr);
        assert(lft->right == rht || rht->left == lft);

        node *join = new node (lft->left, rht->right, 
                               lft->lsite, rht->rsite);
        join->parent = lft->right == rht ? lft->parent : rht->parent;
        join->left->parent = join;
        join->right->parent = join;
        join->trace = new half_edge;
        join->trace->tail = parabola_intersection(join->lsite->site,
                join->rsite->site, y);

        delete lft;
        delete rht;
        delete arc;
        return tmp;
    }

private:
    node *root;
};

struct node {
    // leaf node constructor
    node(pnt p) 
        : isLeaf{true},
          parent{nullptr},
          circle{nullptr},
          site{p},
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
          

    node* intersection(pnt p, double y) const {
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

    node *lpar() const {
        if (!isLeaf)
            throw std::runtime_error("Only valid for leaf nodes");
        if (parent == nullptr) return nullptr;
        if (parent->rsite == this) return parent;
        if (parent->parent == nullptr) return nullptr;
        if (parent->parent->rsite == this) return parent->parent;
        return nullptr;
    }

    node *rpar() const {
        if (!isLeaf)
            throw std::runtime_error("Only valid for leaf nodes");
        if (parent == nullptr) return nullptr;
        if (parent->lsite == this) return parent;
        if (parent->parent == nullptr) return nullptr;
        if (parent->parent->lsite == this) return parent->parent;
        return nullptr;
    }

    node* prev() const {
        node *tmp = lpar();
        return tmp == nullptr ? nullptr : tmp->lsite;
    }

    node* next() const {
        node *tmp = rpar();
        return tmp == nullptr ? nullptr : tmp->rsite;
    }

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

