#include <iostream>
#include <omp.h>

int main()
{
    omp_lock_t my_lock_1, my_lock_2;

    omp_init_lock(&my_lock_1);
    omp_init_lock(&my_lock_2);

#pragma omp parallel num_threads(2)
    {
        if(omp_get_thread_num() == 0)
            omp_set_lock(&my_lock_1);
        else
            omp_set_lock(&my_lock_2);
        
#pragma omp barrier

        if(omp_get_thread_num() == 0)
            omp_set_lock(&my_lock_2);
        else
            omp_set_lock(&my_lock_1);

        omp_unset_lock(&my_lock_1);
        omp_unset_lock(&my_lock_2);
    }
        
    omp_destroy_lock(&my_lock_1);
    omp_destroy_lock(&my_lock_2);
   
    return 0;
}
