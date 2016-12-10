#include <kruskal.h>

#include <map>
#include <tuple>
#include <algorithm>

int kruskal::find(int i) {
    if(sub[i].parent != i)
        sub[i].parent = find(sub[i].parent);
    return sub[i].parent;
}

void kruskal::join(int x, int y) {
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
}

double kruskal::solve(
        const std::vector<std::pair<point<double>,point<double>>> &edges)
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
    return kruskal_algo(std::move(ord), indexer.size());
}

double kruskal::kruskal_algo(
        std::vector<std::tuple<double,int,int>> &&ord,
        std::size_t N)
{
    sub.resize(ord.size());
    for (std::size_t i = 0; i < ord.size(); ++i)
        sub[i] = nd(i);
    std::sort(ord.begin(), ord.end());
    std::size_t cnt = 0;
    double ret = 0.;
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

