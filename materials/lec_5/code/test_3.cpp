#include <iostream>
#include <vector>
#include "profiler.h"
#include "boost/multi_array.hpp"
#include <random>
#include <algorithm>

int main (int argc, char ** argv) 
{
    typedef boost::multi_array<int, 2> matrix;

    profiler prof;
    int n = 2048;
    int inf = 1E6;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, 10);

    matrix a(boost::extents[n][n]);

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            if(i == j)
                a[i][j] = 0;
            else
            {
                a[i][j] = (int)uni(rng);
    
                if(a[i][j] == 10)
                    a[i][j] = inf;
            }

    prof.tic("floyd_seq");
    for(int k = 0; k < n; ++k)
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                a[i][j] = std::min(a[i][j], a[i][k] + a[k][j]);
    prof.toc("floyd_seq");

    prof.report();

    return 0;
}
