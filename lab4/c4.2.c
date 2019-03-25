#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define  M   4
#define  N   500000

int A[M][N], sum[M];

int total;

/*********************************
   struct timeval {
       time_t      tv_sec;   // current seconds since 00:00:00 of 1970 
       suseconds_t tv_usec;  // microseconds in current second 
   };
   **********************************/

   

void *func(void *arg)        // threads function
{
   int temp, mysum = 0;
   int j, row;
   pthread_t tid = pthread_self(); // get thread ID number

   row = (int)arg;                 // get row number from arg
   printf("thread %d computes sum of row %d : ", row, row);
   mysum = 0;

   for (j=0; j < N; j++)     // compute sum of A[row]in global sum[row]
       mysum += A[row][j];

   sum[row] = mysum;

   printf("thread %d done: sum[%d] = %ld\n", row, row, sum[row]);
   pthread_mutex_lock(&total);
   /************** A CRITICAL REGION *******************/
   temp = total;   // get total
   temp += mysum;  // add mysum to temp  
   
   sleep(1);       // OR for (int i=0; i<100000000; i++); ==> switch threads

   total = temp;   //  write temp to total
   /************ end of CRITICAL REGION ***************/
   pthread_mutex_unlock(&total);
   pthread_exit((void*)0);  // thread exit: 0=normal termination
}

// print the matrix (if N is small, do NOT print for large N)
int print()
{
   int i, j;
   for (i=0; i < M; i++){
     for (j=0; j < N; j++){
//       printf("%4d ", A[i][j]);
     }
     printf("\n");
   }
}

int main (int argc, char *argv[])
{
   pthread_mutex_t m;
   pthread_mutex_init(&m, NULL);
   struct timeval t1, t2;
   gettimeofday(&t1, NULL);
   
   pthread_t thread[M];      // thread IDs
   int i, j, status;

   printf("main: initialize A matrix\n");

   for (i=0; i < M; i++){
     for (j=0; j < N; j++){
       A[i][j] = i + j + 1;
     }
   }

   print();

   printf("main: create %d threads\n", M);
   for(i=0; i < M; i++) {
      pthread_create(&thread[i], NULL, func, (void *)i); 
   }

   printf("main: try to join with threads\n");
   for(i=0; i < M; i++) {
     pthread_join(thread[i], (void *)&status);
     printf("main: joined with thread %d : status=%d\n", i, status);
   }

   printf("main: compute and print total : ");
   for (i=0; i < M; i++)
       total += sum[i];
   printf("tatal = %ld\n", total);
   gettimeofday(&t2, NULL);
   printf("Time in program: sec = %ld usec = %ld\n", (t2.tv_sec - t1.tv_sec), (t2.tv_usec - t1.tv_usec));
   pthread_exit(NULL);
   
 
}
