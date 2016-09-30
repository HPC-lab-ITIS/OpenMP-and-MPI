#include <iostream>
#include <vector>
#include <cmath>
#include "profiler.h"

int sieve(const int64_t n)
{
    std::vector<bool> prime (n+1, true);

    int64_t count = 0;

    prime[0] = prime[1] = false;

    for(int64_t i = 2; i <= n; ++i)
        if(prime[i])
        {
            ++count;

            for(int64_t j = i*i; j <= n; j += i)
                prime[j] = false;
        }
    return count;
}

int sieve_opt(const int64_t n)
{
    std::vector<bool> prime(n+1, true);

    int64_t count = 1;
    
    prime[0] = prime[1] = false;

    for(int64_t i = 3; i <= n; i+=2)
        if(prime[i])
        {
            ++count;

            for(int64_t j = i*i; j <= n; j += i)
                prime[j] = false;
        }

    return count;
}

int main()
{
    const int64_t n = 1E9;
    profiler prof;

    prof.tic("SoE");
    std::cout << sieve(n) << std::endl;
    prof.toc("SoE");

    prof.tic("SoE_opt");
    std::cout << sieve_opt(n) << std::endl;
    prof.toc("SoE_opt");

    prof.report();

    return 0;
}
