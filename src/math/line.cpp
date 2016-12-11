#include <math/point.h>
#include <math/line.h>

#include <cassert>

double line::evalX(double x) const
{
    assert(!vertical); // not meaningful
    return m * x + b;
}

double line::evalY(double y) const
{
    if (vertical)
        return b;
    return (y - b) / m;
}

line perpindicular_bisector(const line &seg)
{
    assert(seg.type == line::lineType::SEGMENT);
    point<double> midpoint ((seg.start.x + seg.end.x / 2),
                            (seg.start.y + seg.end.y / 2));
    double m = - 1.0 / seg.m;
    double b = midpoint.y - m * midpoint.x;
    return line(m,b);
}

