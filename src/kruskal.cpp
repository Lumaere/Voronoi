#include <kruskal.h>

#include <map>
#include <tuple>
#include <algorithm>

namespace kruskal {


double solve(const std::vector<std::pair<point<double>,point<double>>> &edges)
{
    std::vector<std::tuple<double,int,int>> ord;
    std::map<point<double>,int> indexer;
    // assign each point to a specific index
    for (const auto &pr : edges) {
        if (indexer.find(pr.first) == indexer.end())
            indexer.insert(std::make_pair(pr.first, indexer.size()));
        if (indexer.find(pr.second) == indexer.end())
            indexer.insert(std::make_pair(pr.second, indexer.size()));
    }
    for (const auto &pr : edges) {
        double d = distance(pr.first, pr.second);
        ord.emplace_back(d,indexer[pr.first],indexer[pr.second]);
    }
    // tree of size n-1
    return kruskal_algo(ord, indexer.size());
}

}

