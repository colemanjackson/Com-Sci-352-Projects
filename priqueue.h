
#ifndef priqueue_h
#define priqueue_h
#include <stdio.h>
#include <stdlib.h>


/*
*THIS CODE IS A MODIFIED PRIORITY QUEUE TAKEN FROM WIKIPEDIA. IT HAS BEEN MODIFIED TO USE A CUSTOM STRUCT.
*/
typedef struct{
  ucontext_t  *function_context;
    // need time varible.
  double timestamp;
  int pid;
} thread_info;


pri_queue priq_new(int size);
 
void priq_push(pri_queue q, thread_info *data, int pri);
 
/* remove top item. returns 0 if empty. *pri can be null. */
thread_info * priq_pop(pri_queue q, int *pri);
 
/* get the top element without removing it from queue */
thread_info* priq_top(pri_queue q, int *pri);
 
/* this is O(n log n), but probably not the best */
thread_info priq_combine(pri_queue q, pri_queue q2);
 

#endif