#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
    int num = 4;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }

#pragma omp parallel num_threads(num)
    {
        std::cout << "Hello, world from thread number " << omp_get_thread_num() << std::endl;
    }
}
