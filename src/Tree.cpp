#include "Tree.h"
#include "Math.h"

#include <stdexcept>
#include <cassert>
#include <iostream>
#include <iomanip>

std::ostream & operator << (std::ostream &os, const pnt &x)
{
    os << '(' << x.x << ',' << x.y << ')';
    return os;
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

bool node::below(const pnt &p, double y) const {
    if (isLeaf) {
        assert(prev() == nullptr ||
                parabola_intersection(prev()->site, site, y) <= p.x);
        assert(next() == nullptr ||
                parabola_intersection(site, next()->site, y) >= p.x);
        return below_parabola(site, y, p);
    } else {
        double h = parabola_intersection(lsite->site, rsite->site, y);
        if (h < p.x)
            return right->below(p, y);
        else
            return left->below(p, y);
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

node* tree::insert(pnt p, double y)
{
    if (root == nullptr) {
        root = new node(p);
        return root;
    }
    // find arc above the site
    node *abv = root->intersection(p, y);

    if (abv->circle != nullptr) {
        abv->circle->valid = false;
    }

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
        // set interval parents
        if (abv->lpar())
            abv->lpar()->rsite = splitL;
        if (abv->rpar())
            abv->rpar()->lsite = splitR;
        // set tree parents
        if (abv->parent->left == abv) {
            leftE->parent->left = leftE;
        } else {
            leftE->parent->right = leftE;
        }
    } else {
        root = leftE;
    }
    delete abv;
    assert(splitL->next() == nxt && nxt->next() == splitR);
    assert(splitR->prev() == nxt && nxt->prev() == splitL);
    return nxt;
}

node* tree::erase(node *arc, double y)
{
    node *lft = arc->lpar();
    node *rht = arc->rpar();
    // TODO: set voronoi diagram correctly
    /* lft->trace->tail = rht->trace->tail = new vertex (arc->circle->vertex); */

    // we can only delete nodes inside the arc since 'flatness' of parabola in
    // beachline will be determined by distance from directrix so first
    // vertices will always go approach infinity slower
    assert(lft != nullptr && rht != nullptr);

    // TODO: set vertex tail for this half-edge
    /* join->trace = new half_edge; */
    /* join->trace->tail = parabola_intersection(join->lsite->site, */
    /*         join->rsite->site, y); */
    if (lft == arc->parent) {
        /*
         *          rht
         *      ...
         *   lft
         *  /   \
         * T     arc
         */
        auto prev = lft->lsite;
        rht->lsite = prev;
        if (lft->parent != rht) {
            prev->parent = arc->parent->parent;
            arc->parent->parent->right = prev;
        } else {
            rht->left = lft->left;
            lft->left->parent = rht;
        }
        delete lft;
        delete arc;
        return prev;
    } else {
        /*
         * lft
         *    ... 
         *        rht
         *       /   \
         *     arc    T
         */
        auto nxt = rht->rsite;
        lft->rsite = nxt;
        if (rht->parent == lft) {
            lft->right = rht->right;
            rht->right->parent = lft;
        } else {
            nxt->parent = arc->parent->parent;
            arc->parent->parent->left = nxt;
        }
        delete rht;
        delete arc;
        return lft->lsite;
    }
}

void tree::print_tree(node *cur, int indent) const
{
    if (cur != nullptr) {
        std::cout << std::setw(indent) << ' ';
        if (cur->isLeaf)
            std::cout << "LEAF: ";
        else
            std::cout << "BRANCH: ";
        std::cout << cur;
        if (cur->isLeaf)
            std::cout << " " << cur->site << " " << cur->lpar() 
                << " " << cur->rpar() << std::endl;
        else
            std::cout << " " << cur->left << " " << cur->right 
                << " " << cur->lsite->site << " " << cur->rsite->site
                << std::endl;
        print_tree(cur->left, indent + 4);
        print_tree(cur->right, indent + 4);
    }
}

void tree::print_leaves() const { 
    auto cur = root;
    while (cur->left != nullptr)
        cur = cur->left;
    while (cur != nullptr) {
        std::cout << cur->site << ' ';
        cur = cur->next();
    }
    std::cout << std::endl;
}

