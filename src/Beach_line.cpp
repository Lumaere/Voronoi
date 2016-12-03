#include "Beach_line.h"
#include "Event.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>
#include <utility>
#include <tuple>

using pnt = point<double>;

std::ostream & operator << (std::ostream &os, const pnt &x)
{
    os << '(' << x.x << ',' << x.y << ')';
    return os;
}

void beach_line::insert(site p, double y)
{
    if (line.empty()) {
        line.push_back(p);
        return;
    } 
    for (auto it = line.begin(); it != line.end(); ++it) {
        double left = it == line.begin() ? -INF : 
            parabola_intersection(*std::next(it,-1), *it, y);
        double right = std::next(it) == line.end() ? INF:
            parabola_intersection(*it, *std::next(it), y);
        if (left > p.x || p.x > right) continue;
        // found arc above
        line.insert(it, *it);            // break arc above in two
        auto arc = line.insert(it, p);   // insert new arc in between

        check_circle_event(arc, p.y);
        check_circle_event(std::next(arc,-1), p.y);
        check_circle_event(std::next(arc,1), p.y);
        break;
    }
    std::cout << "beach_line: ";
    for (auto s : line)
        std::cout << s << ' ';
    std::cout << std::endl;
}

void beach_line::erase(event *evt)
{
    // TODO: set edges appropriately and mark voronoi vertex

    std::cout << "voronoi vertex: " << evt->vertex.x << " " 
              << evt->vertex.y << std::endl;

    auto nxt = line.erase(evt->arc);

    // check for new circle events
    if (nxt != line.begin())
        check_circle_event(std::next(nxt,-1), evt->prio);
    if (std::next(nxt) != line.end())
        check_circle_event(std::next(nxt), evt->prio);

    std::cout << "beach_line_after: ";
    for (auto s : line)
        std::cout << s << ' ';
    std::cout << std::endl;
}

void beach_line::check_circle_event(std::list<site>::iterator arc, double y)
    const
{
    // set circle event of it to false if any since original circle is now
    // nonempty -> false alarm
    if (arc->e != nullptr && arc->e->prio != y)
        arc->e->valid = false;

    // no circle event possible if less than 3 arcs around position
    if (arc == line.begin() || std::next(arc) == line.end())
        return;

    pnt cent; bool good;
    std::tie(cent,good) = 
        circumcenter(*std::next(arc,-1), *arc, *std::next(arc));
    if (!good) return;
    if (below_beachline(cent, y)) return;
    double r = distance(cent, *arc);
    /* std::cout << "CIRC: " << *std::next(arc,-1) << " " << *arc */ 
    /*     << " " << *std::next(arc) << " " << cent + pnt(0,r) << std::endl; */
    if (cent.y + r > y) {
        // new circle event detected
        arc->e = new event(cent.y + r, event::eventType::CIRCLE, cent, arc);
        pq.push(arc->e);
    }
}


bool beach_line::below_beachline(const point<double> &p, double y) const
{
    for (const auto &para : line)
        if (below_parabola(para, y, p))
            return true;
    return false;
}

