#pragma once

#include "Point.h"
#include "Site.h"
#include "Event.h"
#include "Math.h"
#include "Tree.h"

#include <queue>

class beach_line {
public:
    using pnt = point<double>;

    beach_line(std::priority_queue<event*,
            std::vector<event*>,compareEventPtr> &pq)
        : pq{pq}
    {}

    void insert(site p, double y);
    void erase(event *evt);

private:
    // store reference to main event list
    std::priority_queue<event*,
        std::vector<event*>,compareEventPtr> &pq; 
    // structure to represent beach line -> can switch to better DS such as
    // tree or skip list
    tree line;

    static constexpr double INF = std::numeric_limits<double>::infinity();

    void check_circle_event(node *arc, double y) const;
    bool below_beachline(const point<double> &p, double y) const;
};

