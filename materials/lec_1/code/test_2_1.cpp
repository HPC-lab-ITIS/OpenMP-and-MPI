#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "profiler.h"
#include <cmath>


int main(int argc, char *argv[])
{
    profiler prof;
    std::vector<double> arr(524288,0.);
    int num = 4;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }

    prof.tic("cycle");
    for(int i = 0; i < arr.size(); ++i)
        arr[i] = sqrt( sin(i)*sin(i) + 1. ) + cos(i)/sin(i);
    prof.toc("cycle");

    prof.tic("cycle_omp");
#pragma omp parallel for num_threads(num) if(arr.size()>100000)
    for(int i = 0; i < arr.size(); ++i)
        arr[i] = sqrt(sin(i)*sin(i) + 1.)  + cos(i)/sin(i);
        
    prof.toc("cycle_omp");
    
    prof.report();

    return 0;
}
