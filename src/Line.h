#pragma once

#include "Point.h"

/* represent line using slope and y-intercept
 * line struct is either segment, ray, or line and 'endpoints' are used
 * appropriately */
struct line {
    line(long double m = 0, long double b = 0) 
        : m{m}, b{b}, type{LINE}
    {}

    template<typename T>
    line(point<T> p, long double m)
        : m{m},
          b{static_cast<long double>(p.y) - m * p.x},
          start{static_cast<long double>(p.x), static_cast<long double>(p.y)},
          type{RAY}
    {}

    template<typename T>
    line(point<T> p1, point<T> p2)
        : m{static_cast<long double>((p1.y - p2.y)) / (p1.x - p2.x)},
          b{static_cast<long double>(p1.y) - m * p1.x},
          start{static_cast<long double>(p1.x), static_cast<long double>(p1.y)},
          end{static_cast<long double>(p2.x), static_cast<long double>(p2.y)},
          type{SEGMENT}
    {}

    enum lineType { SEGMENT, RAY, LINE };

    /* up to caller to check if x and y are within range of segment for now */
    long double evalX(long double x) const { return m * x + b; }
    long double evalY(long double y) const { return (y - b) / m; }

    /*
     * member variables
     */
    long double m, b;
    point<long double> start, end;
    lineType type;
};

