#include "Beach_line.h"
#include "Event.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>
#include <utility>
#include <tuple>

using pnt = point<double>;

/* std::ostream & operator << (std::ostream &os, const pnt &x) */
/* { */
/*     os << '(' << x.x << ',' << x.y << ')'; */
/*     return os; */
/* } */

void beach_line::insert(site p, double y)
{
    if (line.empty()) {
        line.insert(p, y);
        return;
    }
    auto pos = line.insert(p, y);
    check_circle_event(pos, y);
    check_circle_event(pos->prev(), y);
    check_circle_event(pos->next(), y);
}

void beach_line::erase(event *evt)
{
    // TODO: set edges appropriately and mark voronoi vertex

    std::cout << "voronoi vertex: " << evt->vertex.x << " " 
              << evt->vertex.y << std::endl;
    auto bef = line.erase(evt->arc, evt->prio);

    check_circle_event(bef, evt->prio);
    if (bef->next() != nullptr)
        check_circle_event(bef->next(), evt->prio);
}

void beach_line::check_circle_event(node *arc, double y)
    const
{
    // set circle event of it to false if any since original circle is now
    // nonempty -> false alarm
    if (arc->circle != nullptr && arc->circle->prio != y)
        arc->circle->valid = false;

    // no circle event possible if less than 3 arcs around position
    if (arc->prev() == nullptr || arc->next() == nullptr)
        return;

    pnt cent; bool good;
    std::tie(cent,good) = 
        circumcenter(arc->prev()->site, arc->site, arc->next()->site);
    if (!good) return;
    double r = distance(cent, arc->site);
    if (below_beachline(cent, y)) return;
    if (cent.y + r > y) {
    /* std::cout << "CIRC: " << cent.x << " " << cent.y + r << " " */ 
    /*     << arc->prev()->site.x << " " << arc->prev()->site.y << " " */
    /*     << arc->site.x << " " << arc->site.y << " " */
    /*     << arc->next()->site.x << " " << arc->next()->site.y << std::endl; */
        // new circle event detected
        arc->circle = new event(cent.y + r, 
                event::eventType::CIRCLE, cent, arc);
        pq.push(arc->circle);
    }
}


bool beach_line::below_beachline(const point<double> &p, double y) const
{
    return line.below(p, y);
}
