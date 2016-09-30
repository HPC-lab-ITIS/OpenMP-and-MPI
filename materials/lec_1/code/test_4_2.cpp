#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
    int num = 4, a = 3;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }

#pragma omp parallel for firstprivate(a)
    for(int i = 0; i < num; ++i)
    {
        a = i + 1;
        printf("Thread %d has a value of a = %d\n", omp_get_thread_num(), a);
    }
    
    std::cout << "firstprivate(a) after the cycle = " << a << std::endl;

#pragma omp parallel for lastprivate(a)
    for(int i = 0; i < num; ++i)
    {
        a = i + 1;
        printf("Thread %d has a value of a = %d\n", omp_get_thread_num(), a);
    }
    
    std::cout << "lastprivate(a) after the cycle = " << a << std::endl;

}
