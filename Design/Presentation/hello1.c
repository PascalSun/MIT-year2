#include <stdio.h>
#include <omp.h>
int main(void)
{

    omp_set_num_threads(8); //I have set the number of threads =4, you can change this. 
    #pragma omp parallel
    printf("Hello, world.\n");
    return 0;
}
