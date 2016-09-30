#include <iostream>
#include <vector>
#include "profiler.h"
#include <random>
#include <algorithm>
#include <numeric>

int main (int argc, char ** argv) 
{
    profiler prof;
    int n = 256*1024*1024;

    prof.tic("seq");
    std::vector<double> elems;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> uni(1., 10.);

    for(int i = 0; i < n; ++i)
        elems.push_back( (double)uni(rng) );

    double test_result = std::accumulate(elems.begin(), elems.end(), 0.) / ((double)n);

    prof.toc("seq");

    prof.report();

    return 0;
}
