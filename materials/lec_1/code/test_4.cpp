#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
    int num = 4, a = 0;

    if (argc>1)
    {
        std::istringstream iss(argv[1]);
        iss >> num;
    }

#pragma omp parallel for 
    for(int i = 0; i < num; ++i)
        a = i + 1;
    
    std::cout << a << std::endl;
}
