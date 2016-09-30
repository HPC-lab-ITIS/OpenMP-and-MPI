#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "profiler.h"
#include <cmath>


int main(int argc, char *argv[])
{
    const int n = 512;

    std::vector<std::vector<double>> mat(n, std::vector<double>(n));

#pragma omp parallel for 
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            mat[i][j] = sin(i + j + 1.);

    return 0;
}
