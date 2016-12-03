#pragma once

#include "Site.h"

#include <list>

struct event {
    enum eventType { SITE, CIRCLE };

    event(long double prio, eventType type, point<double> vertex)
        : prio{prio},
          type{type},
          valid{true},
          vertex{vertex}
    {}

    event(long double prio, eventType type, point<double> vertex, 
            std::list<site>::iterator arc)
        : prio{prio},
          type{type},
          valid{true},
          vertex{vertex},
          arc{arc}
    {}

    bool operator < (const event &r) const { 
        if (prio != r.prio)
            return prio > r.prio; 
        return vertex.x < r.vertex.x;
    }

    long double prio;
    eventType type;
    bool valid;
    point<double> vertex;
    std::list<site>::iterator arc;
};

struct compareEventPtr {
    bool operator () (const event *l, const event *r) const {
        return *l < *r;
    }
};


