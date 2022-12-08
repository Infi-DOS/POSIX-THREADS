#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//Two 1-dimensional arrays of 10^6 numbers  A(1)=1, A(2)=2, … A(10^6 )=10^6 .
//Compute the B(i)=square(A(i))  in: a) single thread b) four threads.
//Check the difference in the execution time.

#define ARRAY_SIZE 1000000                              //the size of your array that you are going to use
#define BILLION 1000000000.0                            // billion is using for time counter

int number_of_threads;                                  // global threads
double A[ARRAY_SIZE], B[ARRAY_SIZE];                    // global arrays

void* creatingarray(void* arg)                          //function that creates array A
{
    int index = *(int*)arg;
    for (int k=index; k<index+ARRAY_SIZE/number_of_threads; k++)
    {
        A[k]=(k+1);
        if (number_of_threads!=1 && k<index+40)         //for printing the first 40 characters
               printf("%c",41+(index*number_of_threads/ARRAY_SIZE));
        //printf("%d\t",index);
        //printf("%13.0f\t",A[k]);
        //printf("%d\n",k+1);
    }
}
void* squaring(void* arg)                              //function that computes array B[i]=(A[i])^2
{
    int index = *(int*)arg;
    for (int k=index; k<index+ARRAY_SIZE/number_of_threads; k++)
    {
        B[k]=(A[k])*(A[k]);
        if (number_of_threads!=1 && k<index+40)       //for printing the first 40 characters
               printf("%c",41+(index*number_of_threads/ARRAY_SIZE));
        //printf("%d\t",index);
        //printf("%13.0f\t",B[k]);
        //printf("%d\n",k+1);
    }
}

 int main(int argc, char* argv[])
 {
     printf("This program is going to compute the: B=(A[i])^2 of an array A with 10^6 elements.\n");
     printf("First we are going to create the array A[i]=i.\n");
     printf("Input the number of threads you are going to use.");
     do
     {
         printf("\nInput 1 or 4: ");
         scanf("%d",&number_of_threads);

     } while(!((number_of_threads==1) || (number_of_threads==4)));  //Input number 1 for 1 thread or 4 for 4 threads

     int i;
     struct timespec start, start2, end,end2;                       // for time counter
     printf("So you will use: %d threads.\n",number_of_threads);

     printf("\nNow we are going to compute A[i].\n");
     pthread_t th[number_of_threads];                               //Creating an array of threads
     printf("Thread computing is going to start with time counter.\n");

     printf("\n___ Counter started ___\n");
     clock_gettime(CLOCK_REALTIME, &start);                         //Counter here is starting
     for (i=0; i<number_of_threads; i++)
     {
         int* a=malloc(sizeof(int));                            // a helps to divide the work of computing into 4 equal computational pieces
         *a = i*(ARRAY_SIZE/number_of_threads);
         if(pthread_create(th+i, NULL, &creatingarray, a) !=0)  //checking if something is going wrong when pthread create
         {
             printf("\n%d",i);
             perror("Failed to create thread!");
             return i+1;
         }//else printf("\nStarted Succesfully!\n");
     }
     for (i=0; i<number_of_threads; i++)
     {
         if(pthread_join(th[i], NULL) !=0)                      //checking if something is going wrong when pthread join
         {
             perror("Failed to join thread!");
             return i+1;
         }//else printf("\nFinished Succesfully!\n");

     }
     clock_gettime(CLOCK_REALTIME, &end);                       //Counter here stops
     printf("\n___ Counter stopped ___\n");
     double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/BILLION;
     printf("\nTotal Elapsed Time: %.6f ms.\n", time_spent*1000.0);

                                                            // next we are using the same logic for computing B[i]=(A[i])^2
                                                            // we could use 4 threads  for all computing more faster
                                                            // with half code, but for educational use weI did it with 2 times of using threads
     printf("\nNow we are going to compute B[i].\n");
     pthread_t th2[number_of_threads];
     printf("Thread computing is going to start with time counter\n");
     printf("\n___ Counter started ___\n");

     clock_gettime(CLOCK_REALTIME, &start2);
     for (i=0; i<number_of_threads; i++)
     {
         int* a=malloc(sizeof(int));
         *a = i*(ARRAY_SIZE/number_of_threads);
         if(pthread_create(th2+i, NULL, &squaring, a) !=0)
         {
             perror("Failed to create thread!");
             return i+1;
         }//else printf("Started Succesfully!\n");
     }
     for (i=0; i<number_of_threads; i++)
     {
         if(pthread_join(th2[i], NULL) !=0)
         {
             perror("Failed to join thread!");
             return i+4;
         }//else printf("Finished Succesfully!\n");
     }
     clock_gettime(CLOCK_REALTIME, &end2);
     printf("\n___ Counter stopped ___\n");
     time_spent = (end2.tv_sec - start2.tv_sec) + (end2.tv_nsec - start2.tv_nsec)/BILLION;
     printf("\nTotal Elapsed Time: %.6f ms\n", time_spent*1000.0);


     return 0;
}


