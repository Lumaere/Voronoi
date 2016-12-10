#pragma once

#include "math/point.h"

#include <vector>
#include <utility>

class kruskal {
public:
    double solve(const std::vector<std::pair<point<double>,point<double>>> &);
private:
    struct nd {
        int parent;
        int rank;

        nd() : parent{0}, rank{0} {}
        nd(int p, int r=0) : parent{p}, rank{r} {}
    };
    std::vector<nd> sub;
    int find(int i);
    void join(int x, int y);

    double kruskal_algo(
            std::vector<std::tuple<double,int,int>> &&ord,
            std::size_t N);
};


