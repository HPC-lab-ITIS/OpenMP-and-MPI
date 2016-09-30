#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char *argv[])
{
    int num = 4;
    std::vector<double> arr(16384, 2.);
    double sum =0.;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }

#pragma omp parallel for shared(sum)
    for(int i = 0; i < arr.size(); ++i)
//#pragma omp critical
        sum += arr[i];

    std::cout << sum << std::endl;

    return 0;
}
