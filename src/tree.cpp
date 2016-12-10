#include <tree.h>
#include <math/math.h>

#include <cassert>
#include <iostream>
#include <iomanip>

node* tree::insert(pnt p, double y)
{
    if (root == nullptr) {
        lo = p.y;
        root = new node(p);
        diagram->face_factory(p);
        return root;
    } else if (p.y == lo) {
        // insertion on same y-value as first element: no parabola exists abv
        init_insertion(p, y);
        return nullptr;
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
        auto tmp = degenerate_insertion(abv->prev(), abv, p, y);
        if (tmp != nullptr)
            return tmp;
        tmp = degenerate_insertion(abv, abv->next(), p, y);
        if (tmp != nullptr)
            return tmp;
    }

    // divide old arc and insert new one in between
    node *splitL = new node (abv->site);
    node *splitR = new node (abv->site);
    node *nxt = new node (p);
    // new arcs
    node *rightE = new node (nxt, splitR, nxt, splitR);
    node *leftE = new node (splitL, rightE, splitL, nxt);

    // these edges grow in 'opposite' ccw direction tracing out the boundary
    rightE->trace = diagram->half_edge_factory();
    leftE->trace = rightE->trace->twin;
    // inserting a new point so new face
    rightE->trace->left = diagram->face_factory(p);
    leftE->trace->left = diagram->face_of(abv->site);

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

void tree::init_insertion(pnt p, double y)
{
    // insertion on same y-value as first element: no parabola exists abv
    if (root->isLeaf) {
        node *lft, *rht;
        if (root->site.x < p.x) {
            lft = root;
            rht = new node (p);
        } else {
            lft = new node (p);
            rht = root;
        }
        node *par = new node (lft, rht, lft, rht);
        par->trace = diagram->half_edge_factory();
        par->trace->left = diagram->face_factory(std::min(p, root->site));
        par->trace->twin->left = diagram->face_of(std::max(p, root->site));
        root = par;
    } else {
        node *cur = root;
        while (cur->left != nullptr)
            cur = cur->left;
        // we originally sorted so same y's have x's from hi->lo
        assert(cur->site.x > p.x);
        assert(cur->rpar() == cur->parent);
        node *nxt = new node (p);
        node *tmp = cur->parent;
        node *par = new node (nxt, cur, nxt, cur);
        par->parent = tmp;
        tmp->left = par;
        par->trace = diagram->half_edge_factory();
        par->trace->left = diagram->face_factory(p);
        par->trace->twin->left = diagram->face_of(cur->site);
    }
}

node* tree::degenerate_insertion(node *abv, node *nxt, pnt p, double y)
{
    if (abv == nullptr || nxt == nullptr)
        return nullptr;
    double inter = parabola_intersection(abv->site, nxt->site, y);
    if (std::fabs(inter - p.x) > EPS)
        return nullptr;

    if (abv->circle) {
        abv->circle->valid = false;
        abv->circle = nullptr;
    }
    if (nxt->circle) {
        nxt->circle->valid = false;
        nxt->circle = nullptr;
    }

    node* sep = abv->rpar();
    DCEL::face* lft = diagram->face_of(abv->site);
    DCEL::half_edge* from = sep->trace->left == lft ? 
        sep->trace : sep->trace->twin;

    assert(sep->trace->left == lft || sep->trace->twin->left == lft);
    assert(from->tail == nullptr);
    assert(from->left && from->twin->left);

    // do some bookkeeping on the tree
    node *tmp = sep->right;
    node *new_site = new node (p);
    sep->right = new node (new_site, tmp, new_site, nxt);
    sep->right->parent = sep;
    sep->rsite = new_site;

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
    from->tail = diagram->vertex_factory(
            pnt(inter, parabola_val(abv->site, y, inter)));
    DCEL::half_edge *lnxt = diagram->half_edge_factory();
    DCEL::half_edge *rnxt = diagram->half_edge_factory();
    lnxt->set_next(from);
    rnxt->set_prev(from->twin);
    rnxt->tail = from->tail;
    lnxt->twin->tail = from->tail;
    lnxt->twin->left = diagram->face_factory(p);
    rnxt->twin->set_next(lnxt->twin);

    sep->trace = lnxt;
    sep->right->trace = rnxt;
    assert(new_site->next() == nxt && new_site->prev() == abv);
    assert(abv->next() == new_site && nxt->prev() == new_site);
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

DCEL::half_edge* tree::add_endpoints(node *lftB, node *rhtB, node *arc)
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

    DCEL::half_edge *toP, *fromP;
    std::tie(toP, fromP) = match_face(lftB->trace, rhtB->trace);
    assert(toP->left == fromP->left);
    assert(toP->next == nullptr);
    toP->next = fromP;
    fromP->prev = toP;
    // new voronoi vertex at this point
    assert(fromP->tail == nullptr && toP->twin->tail == nullptr);
    fromP->tail = toP->twin->tail = 
        diagram->vertex_factory(arc->circle->vertex);
    // set representative
    fromP->tail->rep = fromP;

    // continue edge & twin from new intersection and set pointers
    DCEL::half_edge *ret = diagram->half_edge_factory();
    ret->set_next(toP->twin);
    ret->twin->set_prev(fromP->twin);
    ret->twin->tail = fromP->tail;
    return ret;
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

