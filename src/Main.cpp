#include <DCEL/DCEL.h>
#include <Fortunes.h>
#include <kruskal.h>

#include <cassert>
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>
#include <fstream>

void test_voronoi1();
void test_voronoi2();
void test_voronoi3();

int main(int argc, char ** argv)
{
    test_voronoi2();
}

void test_voronoi1()
{
    std::vector<point<double>> t 
        { {84, 125}, {36, 111}, {97, 125}, {82, 149}, {19, 153}, {61, 167}};
    DCEL* v = fortunes_algorithm(t);
    v->print_dcel();
    std::vector<DCEL::half_edge*> holdE;
    std::tie(holdE,std::ignore,std::ignore) = v->to_vector();
    std::vector<std::pair<point<double>,point<double>>> out;
    std::set<DCEL::half_edge*> s;
    for (auto e : holdE) {
        if (s.count(e)) continue;
        if (s.count(e->twin)) continue;
        out.emplace_back(e->left->site, e->twin->left->site);
        s.insert(e);
        s.insert(e->twin);
    }
    std::cout << kruskal::solve(out) << std::endl;
    delete v;
}

void test_voronoi2()
{
    std::vector<point<double>> t 
    {
        {131, 189}, {61,  68}, {171, 137}, {125, 156}, {102, 120},
        {104,  98}, {93, 116}, {170, 161}, {163,  67}, {134,  81},
        {127,  79}, {51, 124}, {123,  78}, {65,  86}, {184, 165},
        {55, 176}, {152, 196}, {82, 190}, {157, 109}, {89,  76},
        {73, 182}, {71, 140}, {77,  94}, {53, 193}, {188,  72},
        {164,  97}, {169, 133}, {103, 128}, {91, 194}, {83,  50},
        {175,  70}, {132,  95}, {139,  62}, {110, 130}, {172, 114},
        {180, 107}, {167, 101}, {143,  60}, {144, 129}, {64,  96},
        {168,  59}, {162,  87}, {166,  56}, {200, 135}, {75, 111},
        {147, 159}, {112, 115}, {57, 105}, {149,  99}, {117, 118},
        {84, 100}, {178, 186}, {183, 199}, {108, 198}, {142, 150},
        {80, 179}, {160, 195}, {197, 126}, {155, 154}, {63, 138},
        {54,  58}, {191, 145}, {185, 122}, {148, 151}, {136, 173},
        {174,  66}, {88,  69}, {146, 121}, {106, 119}, {153, 192},
        {181, 187}, {92,  74}, {85, 113}, {141,  90}, {52, 177}
    };
    auto v = fortunes_algorithm(t);
    v->print_dcel();
    std::vector<DCEL::half_edge*> holdE;
    std::tie(holdE,std::ignore,std::ignore) = v->to_vector();
    std::vector<std::pair<point<double>,point<double>>> out;
    std::set<DCEL::half_edge*> s;
    for (auto e : holdE) {
        if (s.count(e)) continue;
        if (s.count(e->twin)) continue;
        out.emplace_back(e->left->site, e->twin->left->site);
        s.insert(e);
        s.insert(e->twin);
    }
    std::cout << kruskal::solve(out) << std::endl;
    delete v;
}

void test_voronoi3()
{
    std::vector<point<double>> t;
    std::ifstream in ("input/tmp.txt");
    double x, y;
    while (in >> x >> y)
        t.emplace_back(x, y);
    std::sort(t.begin(), t.end());
    t.erase(std::unique(t.begin(), t.end()), t.end());
    auto v = fortunes_algorithm(t);
    std::vector<DCEL::half_edge*> holdE;
    std::tie(holdE,std::ignore,std::ignore) = v->to_vector();
    std::vector<std::pair<point<double>,point<double>>> out;
    std::set<DCEL::half_edge*> s;
    for (auto e : holdE) {
        if (s.count(e)) continue;
        if (s.count(e->twin)) continue;
        out.emplace_back(e->left->site, e->twin->left->site);
        s.insert(e);
        s.insert(e->twin);
    }
    std::cout << kruskal::solve(out) << std::endl;
    delete v;
}

