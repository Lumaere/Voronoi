#include "Fortunes.h"
#include "event.h"
#include "beach_line.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <cassert>


/* template <typename T> */
std::vector<line> fortunes_algorithm(std::vector<point<double>> P)
{
    static constexpr double INF = std::numeric_limits<double>::infinity();
    std::priority_queue<event*,std::vector<event*>,compareEventPtr> q;
    beach_line line (q);
    std::pair<point<double>,point<double>> bounds;
    bounds.first = {INF, INF};
    bounds.second = {-INF, -INF};
    for (auto p : P) {
        q.push(new event(p.y,event::eventType::SITE,p));
        bounds.first.x = std::min(bounds.first.x, p.x);
        bounds.first.y = std::min(bounds.first.y, p.y);
        bounds.second.x = std::max(bounds.second.x, p.x);
        bounds.second.y = std::max(bounds.second.y, p.y);
    }

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

    // resize bounds to have better viewing
    double dx = (bounds.second.x - bounds.first.x + 1) / 5.0;
    double dy = (bounds.second.y - bounds.first.y + 1) / 5.0;
    bounds.first.x -= dx; bounds.first.y -= dy;
    bounds.second.x -= dx; bounds.second.y -= dy;
    std::cerr << holdE.size() << " " << holdV.size() << std::endl;

    for (auto e : holdE) {
        if (e->prev) {
            assert(e->prev->next == e);
            assert(e->prev->left == e->left);
        }
        if (e->next) {
            assert(e->next->prev == e);
            assert(e->next->left == e->left);
        }
        assert(e->twin->twin == e);
        if (e->tail)
            assert(e->twin->next->tail == e->tail &&
                    e->prev->twin->tail == e->tail);
    }

    /* for (auto e : holdE) { */
    /*     if (e->tail && e->twin->tail) { */
    /*     double x1 = e->tail->coordinates.x, y1 = e->tail->coordinates.y; */
    /*     double x2 = e->twin->tail->coordinates.x, y2 = e->twin->tail->coordinates.y; */
    /*     if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) continue; */
    /*     if (x1 > 1000 || x2 > 1000 || y1 > 1000 || y2 > 1000) continue; */
    /*         std::cout << e->tail->coordinates.x << ' ' */ 
    /*             << e->tail->coordinates.y << '\n' */
    /*             << e->twin->tail->coordinates.x << ' ' */
    /*             << e->twin->tail->coordinates.y << "\n\n"; */
    /*     } */
    /* } */

    return {};
}

