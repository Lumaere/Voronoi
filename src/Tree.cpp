#include "Tree.h"
#include "Math.h"

#include <stdexcept>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>

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

node* tree::insert(pnt p, double y)
{
    if (root == nullptr) {
        root = new node(p);
        root->first = true;
        return root;
    }
    // find arc above the site
    node *abv = root->intersection(p, y);

    // set circle event to false since new site appeared in interior of said
    // circle event
    if (abv->circle != nullptr) {
        abv->circle->valid = false;
        abv->circle = nullptr;
    }

    // check for degenerate case
    if (abv->parent != nullptr) {
        if (abv->prev() != nullptr) {
            auto tmp = degenerate_insertion(abv->prev(), abv, p, y);
            if (tmp != nullptr)
                return tmp;
        }
        if (abv->next() != nullptr) {
            auto tmp = degenerate_insertion(abv, abv->next(), p, y);
            if (tmp != nullptr)
                return tmp;
        }
    }

    // divide old arc and insert new one in between
    node *splitL = new node (abv->site);
    node *splitR = new node (abv->site);
    node *nxt = new node (p);

    // bookkeeping and new arcs
    node *rightE = new node (nxt, splitR, nxt, splitR);
    nxt->parent = rightE;
    splitR->parent = rightE;
    node *leftE = new node (splitL, rightE, splitL, nxt);
    splitL->parent = leftE;
    rightE->parent = leftE;

    // these edges grow in 'opposite' ccw direction tracing out the boundary
    // between two sites
    rightE->trace = half_edge_factory();
    leftE->trace = rightE->trace->twin;
    /* inserting a new point so new face */
    rightE->trace->left = new face (p);
    if (abv->first) {
        /* new face because no edges previously to mark */
        abv->first = false;
        leftE->trace->left = new face (abv->site);
    } else {
        leftE->trace->left = face::face_of(abv->site);
    }

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

node* tree::degenerate_insertion(node *abv, node *nxt, pnt p, double y)
{
    double inter = parabola_intersection(abv->site, nxt->site, y);
    if (std::fabs(inter - p.x) > EPS)
        return nullptr;

    node *sep = abv->rpar();
    face *lft = face::face_of(abv->site);
    half_edge *from = sep->trace->left == lft ? sep->trace : sep->trace->twin;

    assert(sep->trace->left == lft || sep->trace->twin->left == lft);
    assert(from->tail == nullptr && from->twin->tail != nullptr);
    assert(from->left && from->twin->left);

    // do some bookkeeping on the tree
    node *tmp = sep->right;
    node *new_site = new node (p);
    sep->right = new node (new_site, tmp, new_site, nxt);
    sep->rsite = new_site;
    new_site->parent = sep->right;
    sep->right->parent = sep;
    tmp->parent = sep->right;

    // set voronoi diagram appropriately
    /*
     *         ^|
     *         || <-from->twin
     *   from> ||
     *         |v
     *        VERT
     *       ^/  ^\
     * lnxt>//    \\ <rnxt
     *     //      \\
     *    /v        \v
     */
    from->tail = new vertex (pnt(inter, parabola_val(abv->site, y, inter)));
    half_edge *lnxt = half_edge_factory();
    half_edge *rnxt = half_edge_factory();
    lnxt->set_next(from);
    rnxt->set_prev(from->twin);
    rnxt->tail = from->tail;
    lnxt->twin->tail = from->tail;
    lnxt->twin->left = new face (p);
    rnxt->twin->set_next(lnxt->twin);

    sep->trace = lnxt;
    sep->right->trace = rnxt;
    if (sep->circle) {
        sep->circle->valid = false;
        sep->circle = nullptr;
    }
    
    return new_site;
}

node* tree::erase(node *arc, double y)
{
    node *lft = arc->lpar();
    node *rht = arc->rpar();
    node *ret;

    // we can only delete nodes inside the arc since 'flatness' of parabola in
    // beachline will be determined by distance from directrix so first
    // vertices will always go approach infinity slower
    assert(lft != nullptr && rht != nullptr);
    assert(arc->circle->valid);

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
            lft->left->parent = arc->parent->parent;
            arc->parent->parent->right = lft->left;
        } else {
            rht->left = lft->left;
            lft->left->parent = rht;
        }
        rht->trace = add_endpoints(lft, rht, arc);
        delete arc;
        delete lft;
        ret = prev;
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
            rht->right->parent = arc->parent->parent;
            arc->parent->parent->left = rht->right;
        }
        lft->trace = add_endpoints(lft, rht, arc);
        delete arc;
        delete rht;
        ret = lft->lsite;
    }
    return ret;
}

half_edge* tree::add_endpoints(node *lftB, node *rhtB, node *arc)
{
    /*
     *  ^\ fromP> ^/
     *   \\ <toP //
     *    \\    //
     *     \v  /v
     *       ^|
     *       || <- ret->twin
     * ret-> ||
     *       |v
     */

    half_edge *toP, *fromP;
    std::tie(toP, fromP) = match_face(lftB->trace, rhtB->trace);
    assert(toP->left == fromP->left);
    assert(toP->next == nullptr);
    toP->next = fromP;
    fromP->prev = toP;
    // new voronoi vertex at this point
    assert(fromP->tail == nullptr && toP->twin->tail == nullptr);
    fromP->tail = toP->twin->tail = new vertex (arc->circle->vertex);
    // set representative
    fromP->tail->rep = fromP;

    // continue edge & twin from new intersection and set pointers
    half_edge *ret = half_edge_factory();
    ret->set_next(toP->twin);
    ret->twin->set_prev(fromP->twin);
    ret->twin->tail = fromP->tail;
    return ret;
}

std::pair<half_edge*,half_edge*> tree::match_face(half_edge *l, half_edge *r)
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
        if (cur->circle)
            std::cout << "collapsing at " << cur->circle->vertex << ' ';
        cur = cur->next();
    }
    std::cout << std::endl;
}

