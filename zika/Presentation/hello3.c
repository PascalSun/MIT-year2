#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
int nthreads, tid;
omp_set_num_threads(4); 
int k =0;
/* Fork a team of threads giving them their own copies of variables */

#pragma omp parallel for  private(nthreads, tid) shared (k)
for(k=0;k<8;k++)
  {

  /* Obtain thread number */
  tid = omp_get_thread_num();
  printf("Hello World from thread = %d and %d\n", tid,k);


  }  /* All threads join master thread and disband */

}

