#pragma once

struct event;

struct site {
    double x, y;
    event *e;

    template <typename T>
    site(point<T> p)
        : x{static_cast<double>(p.x)},
          y{static_cast<double>(p.y)},
          e{nullptr}
    {}

    operator point<double>() const { return {x, y}; }
};

