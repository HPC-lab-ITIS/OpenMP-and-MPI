#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <cmath>
#include "profiler.h"
    
int main(int argc, char *argv[])
{
    int num = 4;
    profiler prof;
    std::vector<double> arr(16384*2048, 0.);
    double sum_crit_1 = 0., sum_crit_2 = 0., local_sum = 0., sum_atom = 0., sum_seq = 0., sum_red = 0.;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }
#pragma omp parallel for num_threads(num)
    for(int i = 0; i < arr.size(); ++i)
        arr[i] = sin(i)*sin(i);
    
    prof.tic("seq");
    sum_seq = std::accumulate(arr.begin(), arr.end(), 0.);
    prof.toc("seq");

#pragma omp barrier
    
    prof.tic("critical_1");
#pragma omp parallel for num_threads(num)
    for(int i = 0; i < arr.size(); ++i)
#pragma omp critical
        sum_crit_1 += arr[i];
    prof.toc("critical_1");
#pragma omp barrier

    prof.tic("critical_2");
#pragma omp parallel shared(sum_crit_2) private(local_sum) num_threads(num)
    {
        local_sum = 0.;

#pragma omp for
        for(int i = 0; i < arr.size(); ++i)
            local_sum += arr[i];

#pragma omp critical
        sum_crit_2 += local_sum;
    }
    prof.toc("critical_2");
#pragma omp barrier
    
    prof.tic("atomic");
#pragma omp parallel shared(sum_atom) private(local_sum) num_threads(num)
    {
        local_sum = 0.;

#pragma omp for
        for(int i = 0; i < arr.size(); ++i)
            local_sum += arr[i];

#pragma omp atomic
        sum_atom += local_sum;
    }
    prof.toc("atomic");
#pragma omp barrier
    
    prof.tic("reduction");
#pragma omp parallel for reduction(+:sum_red)
    for(int i = 0; i < arr.size(); ++i)
        sum_red += arr[i];
    prof.toc("reduction");
#pragma omp barrier
    
    std::cout << "Sequential sum = " << sum_seq << std::endl;
    std::cout << "Sum with critical naive = " << sum_crit_1 << std::endl;
    std::cout << "Sum with critical = " << sum_crit_2 << std::endl;
    std::cout << "Sum with atomic = " << sum_atom << std::endl;
    std::cout << "Sum with reduction = " << sum_red << std::endl;
    
    prof.report();
    
    return 0;
}
