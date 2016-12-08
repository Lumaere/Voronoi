#pragma once

#include "Point.h"
#include "Site.h"
#include "Event.h"
#include "Math.h"
#include "Tree.h"

#include <queue>
#include <utility>

class beach_line {
public:
    using pnt = point<double>;

    beach_line(std::priority_queue<event*,
            std::vector<event*>,compareEventPtr> &pq)
        : pq{pq}
    {}

    // handle site event
    void insert(site p, double y);

    // handle circle event
    void erase(event *evt);

    // put bounding box and clean up edges
    // input: bottom left and top right corners of bounding box
    void clean_up(const std::pair<pnt,pnt> &);

private:
    // store reference to main event list
    std::priority_queue<event*,
        std::vector<event*>,compareEventPtr> &pq; 
    // structure to represent beach line -> can switch to better DS such as
    // tree or skip list
    tree line;

    static constexpr double INF = std::numeric_limits<double>::infinity();

    void check_circle_event(node *arc, double y) const;
};

