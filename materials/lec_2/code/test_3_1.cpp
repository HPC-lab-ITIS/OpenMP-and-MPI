#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "profiler.h"
    
int main(int argc, char *argv[])
{
    int num = 4;
    profiler prof;
    std::vector<double> arr(16384*512, 2.);
    double sum_crit_1 = 0., sum_crit_2 = 0., local_sum = 0., sum_atom = 0.;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }
    
    prof.tic("critical_1");
#pragma omp parallel for num_threads(num)
    for(int i = 0; i < arr.size(); ++i)
#pragma omp critical
        sum_crit_1 += arr[i];
    prof.toc("critical_1");
    
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

    std::cout << "Sum with critical naive = " << sum_crit_1 << std::endl;
    std::cout << "Sum with critical = " << sum_crit_2 << std::endl;
    std::cout << "Sum with atomic = " << sum_atom << std::endl;
    
    prof.report();
    
    return 0;
}
