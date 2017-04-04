#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

//CITS 5507
//Student ID: 21804416 
//Name: SUN QIANG
//Student ID: 21658782
//Name: WANG YU

// Where to use mpi to speed up:
// 1. sort and find blocks;
// 2. Use mpi to fullfill quicksort

//Where use openmp speed up:

//Here we have four tasks using openmp to speed up
//1. use openmp to sort all columns
//2. use openmp to generate blocks
//3. The key point here, use openmp to sort all the signtures(29333879), the openmp speed up is very useful here
//   At first, I can not sort 29333879 signtures with my computer using only one thread, always core down
//   Then, when I use openmp here, then problem solved.
//4. Divide sorted signatures with different parts, and find collisions with openmp

//Where still need speed up

//There are still some problems we can do to speed up:
//The last column(500), generated 29321366 signtures, and the left 499 colunms generated 12510 signtures
// So when we find blocks, most time spent on find blocks in 500 columns with one thread, the other threads are idle.
// However, the time spent to find the blocks in 500 colunm is not very long;
// And we need to keep this program working for any M*N matrix, so I have not done any improvement here.



// we can change the N & M to make this program suitable for some other N*M matrix
// we can change the dia to other value
// Change the Max value if the number of blocks is larger than that

#define N 500
#define M 4400
#define dia 0.000001
#define Max 30000000
#define core_number 12
#define MASTER 0
#define send_data_tag 2001
#define return_data_tag 2002
// ---------
// The data array to store the data information
// The key array to store key information
// When we want to sort values in each column in data, we only need to sort the seq array to store the sequences 
// Location of data value, not directly modify the data array.
// Signs array to store the signtures for blocks
// Location array to store the row ids and column for corresponding signture, for example 434044402220999499 means
// The block combined by 434,444,222,999 rows from 499 column.
// Indexs to store how many blocks generated 
// Id is the same function as the seq array, to store the location information of the signture

//----------
//readData() and readkey() to read values from txt, generate two arrays
// Generate() is to generate the seq array
// sort() is to sort each column in data
// SelectSort() Bubblesort() are used to sort the values (we can use some other methods)
// Findblock() and findblockcol() are used to generate columns
//Sortsign() and BubbleSortsign() are single thread ways to sort the signtures.
// quicksort() is the method which use multiple threads with openmp to sort the signtures based on quicksort.
// resulttxt() is used to print the outcome with two colunm (signture,id)

static double data[N][M];
static long key[M];
static int seq[N][M];
static long signs[Max];
static long all[Max];
static long location[Max];
static int indexs;
static int id[Max]={-1};
int k=0;
void readData();
void readkey();
void generate();
void sort(int start, int end);
void SelectSort(int a[],int n,double b[]);
void BubbleSort(int a[],int n,double b[]);
void findblock(int begin, int last);
void findblockcol(int i,double x[]);
void Sortsign(int a[],int n, long b[]);  
void BubbleSortsign(int a[],int n,long b[]);
void resulttxt();
void quicksort(long arr[], int low_index, int high_index);
int partition(long * a, int p, int r);



int main (void){


    int my_id, ierr, sender,part_index;;  
    int numtasks, taskid, len, rc;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int partner, message;
    long individual_index;
     
    //initiate MPI
    
    MPI_Status status;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Get_processor_name(hostname, &len);
  
    //pre-process for the data
    readData();
    readkey();
    generate();
    //Find the number of cols each task needs to deal with
    int col_each_task = (int)(500 / (numtasks - 1)) + 1;
  
    if (taskid != MASTER)
    {   
        // sort the cols(From starts to ends ) for the task 
        //And then find the blocks 
        int starts=(taskid - 1) * col_each_task; 
        int ends = taskid * col_each_task;
        if (ends >499)
        {
            ends = 499;
        }
        printf ("MPI task %d has started... From %d col to %d \n", taskid,starts,ends);
        sort(starts,ends);
        findblock(starts,ends);
        printf("Number of blcoks found %d in node-%d \n",indexs, taskid);
        //Send back the number of blocks and signtures array found by the task
        ierr = MPI_Send( &indexs, 1, MPI_INT, MASTER, taskid, MPI_COMM_WORLD);
        if (ierr != MPI_SUCCESS)
            printf("%d: Send signature_number failure\n", taskid);
        ierr = MPI_Send(&signs[0],indexs,MPI_LONG,MASTER, taskid, MPI_COMM_WORLD);
        if (ierr != MPI_SUCCESS)
            printf("%d: Send signature failure\n", taskid);
        

    }
    else
    {  
        //recieve all the blocks and the numbers 
        for(taskid = 1; taskid < numtasks;taskid++) {
            
            ierr = MPI_Recv( &part_index, 1, MPI_INT, MPI_ANY_SOURCE,taskid, MPI_COMM_WORLD, &status);
            if (ierr != MPI_SUCCESS)
                    printf("%d: Receive signatures failure\n", taskid);
            sender = status.MPI_SOURCE;
            ierr = MPI_Recv(&all[indexs], part_index,MPI_LONG,MPI_ANY_SOURCE,taskid,MPI_COMM_WORLD,&status);
            if (ierr != MPI_SUCCESS)
                    printf("%d: Receive signatures failure\n", taskid);
            printf("Partial number of blocks %i returned from process %i\n", part_index, sender);
            indexs += part_index;
         }

        
        printf("All number of blocks %d \n",indexs);
    //sort the signtures and find the collisons
    int i;
    for(i=0;i<indexs;i++){
        id[i]=i;
    }

    // // quicksort for openmp here
    quicksort(all,0,indexs-1);
    
    // //Find collsions and print it to result.txt
    resulttxt();
    // }
    }
    

    MPI_Finalize();

}

//Print out the results into results.txt with (signtures,id)(id generate by four row ids and the column id)

void resulttxt(){
    int left =0;
    int indexsnon=0;
    long where;
    FILE *fp;
    fp = fopen("results.txt","a");
    int i;
    int col=0;
    for(i=0;i<indexs;i++){
        if(all[id[i]]!=all[id[i+1]] && all[id[i]]!=all[id[i-1]]){
             indexsnon=indexsnon+1;
        }
        else{
            fprintf(fp,"%ld %ld\n",all[id[i]],all[id[i]]);
            col=col+1;
        }

    }

    fclose(fp);

    printf("The number of collsions found: %d\n",col);
}


//Read keys

void readkey()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_index = 0;
    int column_index = 0;
    fp = fopen("keys.txt","r");
    if (fp ==NULL)
        exit(EXIT_FAILURE);
    while((read = getline(&line,&len,fp))!=-1)
    {
        char *pch; 
        pch=strtok(line," ");
        while (pch!=NULL)
        {
            sscanf(pch,"%ld",&key[column_index]);
            column_index +=1;
            pch = strtok(NULL," ");

        }
        line_index+=1;

    }

    fclose(fp);
    if(line)
        free(line);

}

//Read data
void readData()
{

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_index = 0;
    int column_index;
    fp = fopen("data.txt","r");

    if (fp ==NULL)
        exit(EXIT_FAILURE);
    while((read = getline(&line,&len,fp))!=-1)
    {
        char *pch;
        column_index=0;
        pch=strtok(line," ,");
        while (pch!=NULL)
        {   sscanf(pch,"%lf",&data[column_index][line_index]);
            column_index +=1;
            pch = strtok(NULL," ,");


        }
        line_index+=1;     
    }
    fclose(fp);
    if(line)
        free(line);
  
}

void generate(){
//Generate an array with N*M, and each a[i] with 0-4399 
    int i,j;
    for(i = 0;i<N;i++)
    {
    for (j =0;j<M;j++)
    {
        seq[i][j]=j;
    }
    }
}


//Sort each column

void sort(int start,int end)
{
   int i,j;
   if (end > 499){
       end = 499;
   }
   ////////Here to change
   #pragma omp parallel for
   for(i = start;i < end;i++){
    // We can use different methods to sort here
      SelectSort(seq[i],M,data[i]);
    // BubbleSort(seq[i],M,data[i]);
  }


}

//Select sort method to sort column
void SelectSort(int a[],int n, double b[])    
{    
    int i,j;    
    for(i=0;i<n-1;i++)              
    {    
        int k=i;                      
        for(j=i+1;j<n;j++) 
                       
            if(b[a[k]]>b[a[j]])  k=j;       
        if(k!=i)    
        {    
            int temp=a[i];  

            a[i]=a[k];    
            a[k]=temp;    
        }    
    } 

    
}  



//Select sort method for signtures
void Sortsign(int a[],int n, long b[])    
{    
    int i,j;    
    for(i=0;i<n-1;i++)              
    {   //printf("%d %ld\n", i,b[i]);
        int k=i;                      
        for(j=i+1;j<n;j++) 
                       
            if(b[a[k]]>b[a[j]])  k=j;       
        if(k!=i)    
        {    
            int temp=a[i];  

            a[i]=a[k];    
            a[k]=temp;    
        }    
    } 

    
} 

//Bubble sort for signtures
void BubbleSortsign(int a[],int n,long b[])  
{  
    int i,j;  
    for(i=n-1;i>0;--i){  
       // printf("%d %ld\n", i,b[i]);       
        for(j=0;j<i;j++){
     
            if(b[a[j]]>b[a[j+1]])  
            {  
                int temp=a[j];  
                a[j]=a[j+1];  
                a[j+1]=temp;  
     
            }  
  
        }
    } 
    printf("ok\n");
}



//Find all blocks
void findblock(int begin, int last){
    ///////Here to change
    int i,j;
    //Use the openmp here to find blocks
    #pragma omp parallel for
    
    //Change i<N-1 to calculate the outcome without last column. i<N to calculate the outcome with last column.
    for(i=begin;i<last;i++){
        findblockcol(i,data[i]);
      }

    }

void findblockcol(int i,double x[]){
    int nums=0;
    int a,c,d;
    long b;
    long sign;
    long uni;

    // FILE *fp;
    // fp=fopen("blocks.txt","a");
    #pragma omp parallel for
    for(a=0;a<M;a++){
        b=a+1;
        while(x[seq[i][b]]-x[seq[i][a]]<dia && b <M && x[seq[i][a]]!=0 && x[seq[i][b]]!=0)
        {   
            c=b+1;
            while(x[seq[i][c]]-x[seq[i][a]]<dia && c<M && x[seq[i][c]]!=0){
                d = c+1;
                while(x[seq[i][d]]-x[seq[i][a]]<dia && d<M && x[seq[i][d]]!=0)
                {   
                    indexs=indexs+1;
                    sign=key[a]+key[b]+key[c]+key[d];
                    signs[indexs-1]=sign;

                    uni=(a*1000000000000+b*100000000+c*10000+d)*1000+i;
                    location[indexs-1]=uni;


                    // fprintf(fp,"%ld %ld \n",sign,uni);


                    d = d+1;
                    nums=nums+1;
                
                }
                c=c+1;
            }
            b=b+1; 
        }

    }
    // fclose(fp);

    // printf("Number found for %d column: %d\n",i+1,nums);
}


int partition(long * a, int p, int r)
{
    int lt[r-p];
    int gt[r-p];
    int i;
    int j;
    int key = a[r];
    int lt_n = 0;
    int gt_n = 0;

#pragma omp parallel for
    for(i = p; i < r; i++){
        if(a[i] < a[r]){
            lt[lt_n++] = a[i];
        }else{
            gt[gt_n++] = a[i];
        }   
    }   

    for(i = 0; i < lt_n; i++){
        a[p + i] = lt[i];
    }   

    a[p + lt_n] = key;

    for(j = 0; j < gt_n; j++){
        a[p + lt_n + j + 1] = gt[j];
    }   

    return p + lt_n;
}

void quicksort(long * a, int p, int r)
{
    int div;

    if(p < r){ 
        div = partition(a, p, r); 
#pragma omp parallel sections
        {   
#pragma omp section
            quicksort(a, p, div - 1); 
#pragma omp section
            quicksort(a, div + 1, r); 

        }
    }
}








