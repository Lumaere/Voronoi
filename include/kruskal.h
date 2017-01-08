#pragma once

#include "math/point.h"

#include <vector>
#include <utility>
#include <functional>
#include <algorithm>

namespace kruskal {

double solve(const std::vector<std::pair<point<double>,point<double>>> &);

template <typename T>
T kruskal_algo(std::vector<std::tuple<T,int,int>> &ord, std::size_t N)
{
    // disjoint set data structure
    struct nd {
        int parent, rank;
        nd() : parent{0}, rank{0} {}
        nd(int p, int r=0) : parent{p}, rank{r} {}
    };
    static std::vector<nd> sub;
    static std::function<int(int)> find = [] (int i) {
        if (sub[i].parent != i)
            sub[i].parent = find(sub[i].parent);
        return sub[i].parent;
    };
    static std::function<void(int,int)> join = [](int x, int y) {
        int xroot = find(x);
        int yroot = find(y);
        if(sub[xroot].rank < sub[yroot].rank) 
            sub[xroot].parent = yroot;
        else if(sub[xroot].rank > sub[yroot].rank)
            sub[yroot].parent = xroot;
        else {
            sub[yroot].parent = xroot;
            sub[xroot].rank++;
        }
    };

    // actual kruskal algorithm
    sub.resize(ord.size());
    for (std::size_t i = 0; i < ord.size(); ++i)
        sub[i] = nd(i);
    std::sort(ord.begin(), ord.end());
    std::size_t cnt = 0;
    T ret = 0;
    for (std::size_t i = 0; i < ord.size() && cnt < N; ++i) {
        const auto &it = ord[i];
        int u = std::get<1>(it), v = std::get<2>(it);
        if (find(u) != find(v)) {
            join(u, v);
            ret += std::get<0>(it);
            ++cnt;
        }
    }
    return ret;
}

};


