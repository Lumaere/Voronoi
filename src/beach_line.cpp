#include "beach_line.h"
#include "event.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>
#include <utility>
#include <tuple>

using pnt = point<double>;
int cnt = 1;

void beach_line::insert(site p, double y)
{
    if (line.empty()) {
        line.insert(p, y);
        return;
    }
    node* pos = line.insert(p, y);
    // case where circle event is impossible
    if (pos == nullptr) return;
    check_circle_event(pos, y);
    check_circle_event(pos->prev(), y);
    check_circle_event(pos->next(), y);
}

void beach_line::erase(event *evt)
{
    node* bef = line.erase(evt->arc, evt->prio);
    check_circle_event(bef, evt->prio);
    if (bef->next() != nullptr)
        check_circle_event(bef->next(), evt->prio);
}

void beach_line::clean_up(const std::pair<pnt,pnt> &)
{
    line.print_leaves();
}

void beach_line::check_circle_event(node *arc, double y)
    const
{
    // set circle event of it to false if any since original circle is now
    // nonempty -> false alarm
    if (arc->circle != nullptr && arc->circle->prio != y) {
        arc->circle->valid = false;
        // set this to nullptr because bad reference after deletion
        arc->circle = nullptr; 
    }

    // no circle event possible if less than 3 arcs around position
    if (arc->prev() == nullptr || arc->next() == nullptr)
        return;

    pnt cent; bool good;
    std::tie(cent,good) = 
        circumcenter(arc->prev()->site, arc->site, arc->next()->site);
    if (!good) return;
    double r = distance(cent, arc->site);
    if (cent.y + r > y) {
        // new circle event detected
        arc->circle = new event(cent.y + r, 
                event::eventType::CIRCLE, cent, arc);
        pq.push(arc->circle);
    }
}

