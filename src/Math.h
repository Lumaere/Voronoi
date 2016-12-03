#pragma once

#include "Point.h"

#include <utility>
#include <tuple>

using pnt = point<double>;

std::pair<pnt,bool> circumcenter(pnt a, pnt b, pnt c);

double distance(pnt p1, pnt p2);

double parabola_intersection(const pnt &v1, const pnt &v2, double p);

bool below_parabola(const pnt &focus, double directrix, const pnt &p);

std::tuple<double,double,double> parabola_equation(
        const pnt &focus, double directrix);

