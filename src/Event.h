#pragma once

#include "Site.h"

struct node;
struct event {
    enum eventType { SITE, CIRCLE };

    event(double prio, eventType type, point<double> vertex)
        : prio{prio},
          type{type},
          valid{true},
          vertex{vertex}
    {}

    event(double prio, eventType type, point<double> vertex, 
            node *arc)
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

    double prio;
    eventType type;
    bool valid;
    point<double> vertex;
    node *arc;
};

struct compareEventPtr {
    bool operator () (const event *l, const event *r) const {
        return *l < *r;
    }
};


