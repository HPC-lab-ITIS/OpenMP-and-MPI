#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
    int n = 4, chunk = 1;

    if (argc > 2)
    {
        std::istringstream iss(argv[1]);
        iss >> n;

        iss.str(std::string());
        iss.clear();
    
        iss.str(argv[2]);
        iss >> chunk;
    }

    if (argc > 3)
    {
        if(argv[3] == "dynamic")
            omp_set_schedule(omp_sched_dynamic, chunk);
        else
            if(argv[3] == "guided")
                omp_set_schedule(omp_sched_guided, chunk);
            else
                omp_set_schedule(omp_sched_static, chunk);
    }
    else
        omp_set_schedule(omp_sched_static, chunk);

#pragma omp parallel for schedule(runtime)
    for(int i = 0; i < n; ++i)
        printf( "Iteration number %d is done by thread %d\n", i, omp_get_thread_num() );
}
