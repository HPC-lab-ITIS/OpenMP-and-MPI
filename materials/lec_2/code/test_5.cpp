#include <iostream>
#include <omp.h>

int hello(int tid)
{
    std::cout << "Lock is set by thread " << tid << std::endl;
    return 0;
}

int main()
{
    omp_lock_t my_lock;
    
    omp_init_lock(&my_lock);

#pragma omp parallel num_threads(4)
    {
        omp_set_lock(&my_lock);
        hello(omp_get_thread_num());
        omp_unset_lock(&my_lock);
    }

    omp_destroy_lock(&my_lock);

    return 0;
}
