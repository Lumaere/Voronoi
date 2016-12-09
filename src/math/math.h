#pragma once

#include "point.h"

#include <utility>
#include <tuple>

using pnt = point<double>;

static constexpr double EPS = 1e-11;

std::pair<pnt,bool> circumcenter(pnt a, pnt b, pnt c);

double parabola_intersection(const pnt &v1, const pnt &v2, double p);

double parabola_val(const pnt &focus, double directrix, double x);

bool below_parabola(const pnt &focus, double directrix, const pnt &p);

std::tuple<double,double,double> parabola_equation(
        const pnt &focus, double directrix);

