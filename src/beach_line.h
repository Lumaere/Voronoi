#pragma once

#include "math/point.h"
#include "math/math.h"
#include "site.h"
#include "event.h"
#include "tree.h"

#include <queue>
#include <utility>

class beach_line {
public:
    using pnt = point<double>;

    beach_line(std::priority_queue<event*,
            std::vector<event*>,compareEventPtr> &pq)
        : diagram{new DCEL},
          pq{pq},
          line(diagram)
    {}

    // handle site event
    void insert(site p, double y);

    // handle circle event
    void erase(event *evt);

    // TODO: put bounding box and clean up edges
    // input: bottom left and top right corners of bounding box
    void clean_up(const std::pair<pnt,pnt> &);

    DCEL* diagram;
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

