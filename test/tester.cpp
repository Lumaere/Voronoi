#include <math/point.h>
#include <math/line.h>
#include <kruskal.h>
#include <DCEL/DCEL.h>
#include <Fortunes.h>

#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>

void test_point();
void test_line();
void test_kruskal_small();
void test_kruskal_small2();
void test_kruskal_medium();
void test_kruskal_large();
void test_voronoi1();
void test_voronoi2();
void test_voronoi3();

int main(int argc, char ** argv)
{
    std::cout << std::fixed << std::setprecision(4);
    test_point();
    test_line();
    test_kruskal_small();
    test_kruskal_small2();
    test_kruskal_medium();
    test_voronoi1();
    test_voronoi2();
    test_voronoi3();
}

void test_point()
{
    point<int> u (1, 2);
    point<int> v (3, -1);
    assert(u + v == point<int>(4, 1));
    assert(u - v == point<int>(-2, 3));
    assert(u * 3 == point<int>(3, 6));
    assert(u * -3 == point<int>(-3, -6));
    assert(u / -3 == point<int>(0, 0));
    assert(u / 2 == point<int>(0, 1));
    assert(u.dot(v) == 1);
    assert(v.dot(u) == u.dot(v));
    assert(u.det(v) == -7);
    assert(v.det(u) == -u.det(v));
    assert(u < v);
}

void test_line()
{
    point<int> u (1, 2);
    point<int> v (3, -1);
    line l (u, v);
    assert(l.m == -1.5);
    assert(l.b == 3.5);
    assert(l.evalX(2) == 0.5);
    assert(l.evalY(0.5) == 2);
}

void assert_kruskal(const std::vector<point<double>> &ps, double res);

void test_kruskal_small()
{
    std::vector<point<double>> ps = {
        {6,1},
        {2,3},
        {-5,-4},
        {-1,6},
        {0,-1}
    };
    assert_kruskal(ps, 19.0179);
}

void test_kruskal_small2()
{
    std::vector<std::tuple<int,int,int>> ps = {
        {3,0,1},
        {9,0,2},
        {6,0,3},
        {4,1,3},
        {9,2,1},
        {2,1,4},
        {2,4,3},
        {8,5,2},
        {9,5,1},
        {8,4,5},
        {9,3,6},
        {9,4,6},
        {7,6,5},
        {4,6,7},
        {1,7,8},
        {4,7,9},
        {5,8,6},
        {9,5,8},
        {3,8,9},
        {10,9,5},
        {18,9,2}
    };
    assert(kruskal::kruskal_algo(ps,10) == 38);
}

void test_kruskal_medium()
{
    std::vector<point<double>> ps = {
        {0,36}, {-24,36}, {30,28}, {43,-26}, {0,-23},
        {-33,-41}, {-31,42}, {-33,15}, {-48,1}, {26,-45},
        {-10,2}, {-15,0}, {2,19}, {-49,27}, {34,14},
        {9,27}, {-9,5}, {-23,-41}, {43,-44}, {-33,-10},
        {-8,41}, {-32,-20}, {-45,-34}, {36,-49}, {47,6},
        {3,-23}, {32,30}, {17,8}, {0,-3}, {39,32}
    };
    assert_kruskal(ps, 397.827);
}

void test_kruskal_large()
{
    std::vector<point<double>> ps;
    std::ifstream dat ("test/kruskal_large_dat.txt");
    if (!dat) {
        std::cerr << "Could not open test file" << std::endl;
        return;
    }
    double x, y;
    while (dat >> x >> y) {
        ps.emplace_back(x,y);
    }
    assert_kruskal(ps, 29445.5345);
}

void assert_kruskal(const std::vector<point<double>> &ps, double res)
{
    std::vector<std::pair<point<double>,point<double>>> edges;
    for (std::size_t i = 0; i < ps.size(); ++i)
        for (std::size_t j = 0; j < ps.size(); ++j)
            if (i != j)
                edges.emplace_back(ps[i], ps[j]);
    assert(std::abs(kruskal::solve(edges) - res) < 0.0001);
}


void assert_voronoi(const std::vector<point<double>> &t, double res);

void test_voronoi1()
{
    std::vector<point<double>> t 
        { {84, 125}, {36, 111}, {97, 125}, {82, 149}, {19, 153}, {61, 167}};
    assert_voronoi(t,154.3238);
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
    assert_voronoi(t,890.6003);
}

void test_voronoi3()
{
    std::vector<point<double>> t;
    std::ifstream in ("test/kruskal_large_dat.txt");
    double x, y;
    while (in >> x >> y)
        t.emplace_back(x, y);
    std::sort(t.begin(), t.end());
    t.erase(std::unique(t.begin(), t.end()), t.end());
    assert_voronoi(t,29445.5345);
}


void assert_voronoi(const std::vector<point<double>> &t, double res)
{
    DCEL* v = fortunes_algorithm(t);
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
    assert(std::abs(kruskal::solve(out) - res) < 0.0001);
    delete v;
}

