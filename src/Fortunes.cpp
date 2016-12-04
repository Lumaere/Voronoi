#include "Fortunes.h"
#include "Event.h"
#include "Beach_line.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <cassert>


/* template <typename T> */
std::vector<line> fortunes_algorithm(std::vector<point<double>> P)
{
    std::priority_queue<event*,std::vector<event*>,compareEventPtr> q;
    beach_line line (q);
    for (auto p : P)
        q.push(new event(p.y,event::eventType::SITE,p));

    while (!q.empty()) {
        auto e = q.top();
        q.pop();
        if (e->valid) {
            if (e->type == event::eventType::SITE) {
                line.insert(e->vertex, e->prio);
            } else {
                line.erase(e);
            }
        }
        delete e;
    }

    for (auto e : hold) {
        std::cout << e << " " << e->prev << " " << e->next;
        if (e->tail)
            std::cout << " tail: " << e->tail->coordinates;
        if (e->head)
            std::cout << " head: " << e->head->coordinates;
        std::cout << std::endl;
    }

    return {};
}

