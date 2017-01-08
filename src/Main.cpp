#include <DCEL/DCEL.h>
#include <Fortunes.h>

#include <vector>
#include <iostream>
#include <algorithm>

int main(int argc, char ** argv)
{
    std::vector<point<double>> t;
    int N;
    double x, y;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        std::cin >> x >> y;
        t.emplace_back(x, y);
    }
    std::sort(t.begin(), t.end());
    t.erase(std::unique(t.begin(), t.end()), t.end());
    DCEL* v = fortunes_algorithm(t);
    v->print_dcel();
    delete v;
}

