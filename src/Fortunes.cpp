#include <Fortunes.h>
#include <event.h>
#include <beach_line.h>

#include <iostream>
#include <queue>

DCEL* fortunes_algorithm(std::vector<point<double>> P)
{
    std::priority_queue<event*,std::vector<event*>,compareEventPtr> q;
    beach_line line (q);
    double lo = std::numeric_limits<double>::infinity();
    double hi = -std::numeric_limits<double>::infinity();
    for (auto p : P) {
        q.push(new event(p.y,event::eventType::SITE,p));
        lo = std::min(lo, p.y);
        hi = std::max(hi, p.y);
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
    line.clean_up((hi + (hi - lo)) * 2);

    return line.diagram;
}

