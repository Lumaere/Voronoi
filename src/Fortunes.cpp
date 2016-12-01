#include "Fortunes.h"
#include "Parabola.h"

#include <limits>
#include <algorithm>
#include <iostream>

/* template <typename T> */
std::vector<line> fortunes_algorithm(std::vector<point<int>> P)
{
    static auto compareY = [] (const point<int> &l, const point<int> &r) 
        { return l.y == r.y ? l.x < r.x : l.y > r.y; };
    std::sort(P.begin(), P.end(), compareY);

    for (auto p : P)
        std::cout << p.x << " " << p.y << std::endl;

    std::vector<parabola> beach_line;
    line sweep_line (0, P.front().y);

    return {};
}

