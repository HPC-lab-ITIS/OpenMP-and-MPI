#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char *argv[])
{
    int num = 4;
    std::vector<double> arr(16384*256, 2.);
    double sum =0., local_sum = 0.;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }

#pragma omp parallel shared(sum) private(local_sum) num_threads(num)
    {
        local_sum = 0.;

#pragma omp for
        for(int i = 0; i < arr.size(); ++i)
            local_sum += arr[i];

//#pragma omp critical
        sum += local_sum;
    }

    std::cout << sum << std::endl;

    return 0;
}
