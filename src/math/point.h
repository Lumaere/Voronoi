#pragma once

#include <ostream>
#include <cmath>

template <typename T>
struct point {
    T x, y;

    /* define copy constructor and move assignment
     * std::sort needs copy construction and move assignment so make sure these
     * aren't implicitly deleted */
    point(const point &r) = default;
    point(point &&r) = default;
    point& operator = (point &&r) = default;

    point(T x = 0, T y = 0) : x{x}, y{y} {}
    point operator + (const point &rhs) const 
        { return { x + rhs.x, y + rhs.y }; }
    point operator - (const point &rhs) const 
        { return { x - rhs.x, y - rhs.y }; }
    point operator * (T k) const { return { x * k, y * k }; }
    point operator / (T k) const { return { x / k, y / k }; }
    double dot(const point &rhs) const { return x * rhs.x + y * rhs.y; }
    double det(const point &rhs) const { return x * rhs.y - y * rhs.x; }
    double abs() const { return x * x + y * y; }
    bool operator < (const point &rhs) const 
        { return x == rhs.x ? y < rhs.y : x < rhs.x; }
    bool operator == (const point &rhs) const 
        { return x - rhs.x == 0 && y - rhs.y == 0; }
};

template <typename T>
std::ostream & operator << (std::ostream &os, const point<T> &x)
{
    os << '(' << x.x << ',' << x.y << ')';
    return os;
}

template <typename T>
T distance(const point<T> &p1, const point<T> &p2)
{
    return sqrt((p1 - p2).abs());
}

