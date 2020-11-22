/*
 *   Sieve of Eratosthenes
 *
 *   Programmed by Michael J. Quinn
 *
 *   Last modification: 7 September 2001
 */

#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define MIN(a,b)  ((a)<(b)?(a):(b))

int main (int argc, char *argv[])
{
   unsigned long int    count;        /* Local prime count */
   double elapsed_time; /* Parallel execution time */
   unsigned long int    first;        /* Index of first multiple */
   int   local_first;
   unsigned long int    global_count = 0; /* Global prime count */
   unsigned long long int    high_value;   /* Highest value on this proc */
   unsigned long int    i;
   int    id;           /* Process ID number */
   unsigned long int    index;        /* Index of current prime */
   unsigned long long int    low_value;    /* Lowest value on this proc */
   char  *marked;       /* Portion of 2,...,'n' */
   char  *local_prime_marked;
   unsigned long long int    n;            /* Sieving from 2, ..., 'n' */
   int    p;            /* Number of processes */
   unsigned long int    proc0_size;   /* Size of proc 0's subarray */
   unsigned long int    prime;
   unsigned long int  local_prime;        /* Current prime */
   unsigned long int    size;         /* Elements in 'marked' */
   unsigned long int  local_prime_size;


   //add on variable
   unsigned long long int    low_value_begin;    /* Lowest value for first set */
   unsigned long long int    high_value_begin;    /* highest value for first set */
   unsigned long int    size_begin;         /* Elements in begin set */
   char  *marked_begin;       /* Portion of 2,...,'n' */
   unsigned long int *prime_list;
   unsigned long int *first_list;
   unsigned char list_size;
   int j;


   MPI_Init (&argc, &argv);

   /* Start the timer */

   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   MPI_Comm_size (MPI_COMM_WORLD, &p);
   MPI_Barrier(MPI_COMM_WORLD);
   elapsed_time = -MPI_Wtime();

   if (argc != 2) {
      if (!id) printf ("Command line: %s <m>\n", argv[0]);
      MPI_Finalize();
      exit (1);
   }

   n = atoll(argv[1]);

   /* Figure out this process's share of the array, as
       well as the integers represented by the first and
       last array elements */

   low_value = floor(3 + id * (n - 2) / p);
   if(!(low_value % 2)) low_value--;
   low_value_begin = 3;


   high_value = floor(2 + (id + 1) * (n - 2) / p);
   if(!(high_value % 2)) high_value++;
   high_value_begin = floor(2 + (n - 2) / p);


   size = (high_value - low_value + 1)/2;
   size_begin = (high_value_begin -low_value_begin +1)/2;
   


   proc0_size = ((n - 2) / p)/2;

   if ((3 + proc0_size) < (int) sqrt((double) n)) {
      if (!id) printf("Too many processes\n");
      MPI_Finalize();
      exit(1);
   }

   /* Allocate this process's share of the array. */

    marked = (char *) malloc(size);
    marked_begin = (char *) malloc(size);

    if (marked == NULL) {
        printf("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit(1);
    }
    if (marked_begin == NULL) {
        printf("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit(1);
    }

    //init the marked array to all 0
   for (i = 0; i < size; i++) marked[i] = 0;
   for (i = 0; i < size_begin; i++) marked_begin[i] = 0;


   index = 0;

   int prime_size;
   prime_size = 3;

   

   // register unsigned  int *a;

   // a = (unsigned int *) malloc(4*prime_size);

   

   // register unsigned  int *f;

   int block_size;
   block_size = 3000;

   register unsigned long int a0;
   register unsigned long int a1;
   register unsigned long int a2;

   register unsigned long int f0;
   register unsigned long int f1;
   register unsigned long int f2;

   

   // f = (unsigned int *) malloc(4*prime_size);

   // if (a == NULL) {
   //      printf("Cannot allocate enough memory\n");
   //      MPI_Finalize();
   //      exit(1);
   //  }

   //  if (f == NULL) {
   //      printf("Cannot allocate enough memory\n");
   //      MPI_Finalize();
   //      exit(1);
   //  }

   list_size = 0;


   //find all the prime in the beginning
   prime = 3;
   do {
      //to put the value into the prime list
      if(list_size%3 == 0){
         a0 = prime;
      }
      else if(list_size%3 == 1){
         a1 = prime;
      }
      else{
         a2 = prime;
      }
      // a[list_size%prime_size] = prime;

      
      //use to mark all the prime in process not equal to 0
      if(id != 0){
         if (prime * prime > low_value)
            first =( prime * prime - low_value ) /2;
         else {
            if (!(low_value % prime)) first = 0;
            else{
               if((low_value % prime)%2 == 0){
                  first = (2 * prime - low_value % prime) / 2;
               }
               else{
                  first = (prime - low_value % prime)/2;
               }
            }
         }

         if(list_size%3 == 0){
            f0 = first;
         }
         else if(list_size%3 == 1){
            f1 = first;
         }
         else{
            f2 = first;
         }
         // f[list_size%prime_size] = first;
         list_size++;

         // if(p == 32){
         //    if(id == 1){
         //       if(list_size%100 == 0){
         //          printf("list size reach 19 with prime: %lu\n",prime);
         //       }
         //    }
         // }
         

         

         if(list_size%prime_size == 0){


            i = low_value + block_size;
            while(i <= high_value){
               
               while(f0 < i && f0 < size){
                  marked[f0] = 1;
                  f0 += a0;
               }
               while(f1 < i && f1 < size){
                  marked[f1] = 1;
                  f1 += a1;
               }
               while(f2 < i && f2 < size){
                  marked[f2] = 1;
                  f2 += a2;
               }

               // for(j = 0; j < prime_size; j++){
               //    for(;(f[j] <i) &&(f[j] < size); f[j] += a[j]){
               //       marked[f[j]] = 1;
               //    }
               // }
               


               if(i == high_value){
                  i++;
               }
               else{
                  i = ((i + block_size) > high_value ) ? high_value : i + block_size;
               }
            }

            // list_size = 0;

         }

      }


      //process the init list to find all the next prime to process
      first =( prime * prime - low_value_begin ) /2;
        
      for (i = first; i < size; i += prime) marked_begin[i] = 1;

      while (marked_begin[++index]);
      prime = index*2 + 3;

   } while (prime * prime <= n);


   

   // i = low_value + block_size;
   // while(i <= high_value){
      

   //    for(j = 0; j < list_size%prime_size; j++){
   //       for(;(f[j] <i) &&(f[j] < size); ){
   //          marked[f[j]] = 1;
   //          f[j] += a[j];
   //       }
   //    }
      


   //    if(i == high_value){
   //       i++;
   //    }
   //    else{
   //       i = ((i + block_size) > high_value ) ? high_value : i + block_size;
   //    }
   // }

   
   
   

   count = 0;

   if(id == 0){
      for (i = 0; i < size; i++)
         if (!marked_begin[i]) count++;
   }
   else{
      for (i = 0; i < size; i++)
         if (!marked[i]) count++;
   }
   

   // if(p == 32){
   //    printf("Before MPI reduce: total time: %10.6f, id = %llu\n",elapsed_time + MPI_Wtime(), id);
   // }
   if (p > 1)
      MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM,
                  0, MPI_COMM_WORLD);

   // unsigned long int global_size = 0;
   // if (p > 1)
   //    MPI_Reduce(&size, &global_size, 1, MPI_INT, MPI_SUM,
   //                0, MPI_COMM_WORLD);




   //add the prime number 2 back
   global_count++;








   /* Stop the timer */

   elapsed_time += MPI_Wtime();


   /* Print the results */

   if (!id) {
      printf("The total number of prime: %ld, total time: %10.6f, total node %d\n", global_count, elapsed_time, p);

   }
   MPI_Finalize ();
   return 0;
}

