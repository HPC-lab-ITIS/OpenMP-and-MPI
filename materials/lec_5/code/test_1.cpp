#include <iostream>
#include <vector>
#include "profiler.h"
#include <random>
#include <algorithm>

int main (int argc, char ** argv) 
{
    profiler prof;
    long int n = 1E8;

    std::random_device rd_1;
    std::random_device rd_2;
    std::mt19937 rng_x(rd_1());
    std::mt19937 rng_y(rd_2());
    std::uniform_real_distribution<double> dist(0., 1.);

    double x, y;
    int count = 0;

    prof.tic("seq_pi");
    for(auto i = 0; i < n; ++i)
    {
        x = (double)dist(rng_x);
        y = (double)dist(rng_y);

        if(x*x + y*y <= 1.)
            ++count;
    }

    double pi = 4. * (double)count / (double)n;
    prof.toc("seq_pi");

    std::cout << pi << std::endl;

    prof.report();

    return 0;
}
