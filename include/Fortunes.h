#pragma once

#include "math/line.h"
#include "math/point.h"
#include "DCEL/DCEL.h"

#include <vector>

/*
 * Returns doubly connected edge list for voronoi diagram of given sites.
 * Notes: does not work with overlapping sites.
 */
DCEL* fortunes_algorithm(std::vector<point<double>> P);

