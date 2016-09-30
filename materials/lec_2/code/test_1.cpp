#include <iostream>
#include <omp.h>

int main()
{
    int a = 0;

#pragma omp parallel shared(a)
    {
        a += 2;

//#pragma omp barrier
        if (omp_get_thread_num() == 0)
            a *= 3;
    }

    std::cout << "a = " << a << std::endl;

    return 0;
}
