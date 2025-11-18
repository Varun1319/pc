#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long long fib_task(int n)
{
    if (n < 2)
        return n;
    long long x, y;
#pragma omp task shared(x)
    x = fib_task(n - 1);
#pragma omp task shared(y)
    y = fib_task(n - 2);
#pragma omp taskwait
    return x + y;
}
int main()
{
    int n;
    printf("Enter number of Fibonacci numbers to compute: ");
    scanf("%d", &n);
    long long *fib_array = (long long *)malloc(n * sizeof(long long));
#pragma omp parallel
    {
#pragma omp single
        {
            for (int i = 0; i < n; i++)
            {

#pragma omp task shared(fib_array)
                fib_array[i] = fib_task(i);
            }
        }
    }

#pragma omp taskwait
    printf("Fibonacci numbers:\n");
    for (int i = 0; i < n; i++)
    {
        printf("fib(%d) = %lld\n", i, fib_array[i]);
    }
    free(fib_array);
    return 0;
}


steps to run:
gcc -fopenmp fib_openmp_tasks.c -o fib_tasks
./fib_tasks
