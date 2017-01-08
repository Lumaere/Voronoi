#pragma once

#include <math/point.h>
#include <limits>

/* represent line using slope and y-intercept
 * line struct is either segment or line and 'endpoints' are used
 * appropriately 
 *
 * NOTE: if vertical, (i.e. x = 10), b = x-intercept */
struct line {
    line(double m = 0, double b = 0) 
        : m{m}, b{b}, type{LINE}
    {
        vertical = m == std::numeric_limits<double>::infinity();
    }

    template<typename T>
    line(point<T> p1, point<T> p2)
        : vertical{p1.x == p2.x},
          m{static_cast<double>((p1.y - p2.y)) / (p1.x - p2.x)},
          b{static_cast<double>(p1.y) - m * p1.x},
          start{static_cast<double>(p1.x), static_cast<double>(p1.y)},
          end{static_cast<double>(p2.x), static_cast<double>(p2.y)},
          type{SEGMENT}
    {
        if (vertical) {
            m = std::numeric_limits<double>::infinity();
            b = p1.x;
        }
    }

    enum lineType { SEGMENT, RAY, LINE };

    /* up to caller to check if x and y are within range of segment for now */
    double evalX(double x) const;
    double evalY(double y) const;

    /*
     * member variables
     */
    bool vertical;
    double m, b;
    point<double> start, end;
    lineType type;
};

line perpindicular_bisector(const line &seg);

