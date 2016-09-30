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

#pragma omp parallel for private(a)
    for(int i = 0; i < num; ++i)
    {
        a += i;
        printf("Thread %d has a value of a = %d\n", omp_get_thread_num(), a);
    }
    
    std::cout << a << std::endl;
}
