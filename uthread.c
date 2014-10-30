#include <uthread.h>
#include <semaphore.h>
#include <ucontext.h>
#include <sys/time.h>
#include <sys/resource.h>

/*
* DEFINE GLOBAL VARIABLES
*/
int max_kernel_thread;
sem_t thread_lock;
thread_info *head;


typedef struct{
  ucontext_t  *function_context;
    // need time varible.
  double timestamp;
  thread_info *next;
  int pid;
} thread_info;

int count_of_linked_list(thread_info *head_of_list)
{
  int index, count = 0;

  while(head->next != NULL)
  {
    count++

  }

  return count;
}


void system_init(int max_number_of_klt)
{
	*head = NULL;
	max_kernel_thread = max_number_of_klt;
  sem_init(&thread_lock, 0, max_kernel_thread);
}


int uthread_create(void (*func)())
{

  int currently_running = 0;
  ucontext_t *process;
  process = (ucontext_t*) malloc(sizeof(ucontext_t));
  process->uc_stack.ss_sp = malloc(16384);
  process->uc_stack.ss_size = 16384;
  makecontext(process, func);
  int error = sem_getValue(&thread_lock, currently_running);
  if(error == -1)
  {
    return -1; // semephore wasn't inintalized. 
  }

  //If the head pointer of the queue is null, initalize the process to the head pointer
  if(head == NULL)
  {
    head = malloc(sizeof(thread_info));
    head->function = process;
  }



  if(currently_running < max_kernel_thread)
  {
    
    
  }

  else
  {

  }

  
  
  

  //make ucontext to the function being passed in.
  //if there are available kernel threads, set the ucontext (run the context) and increment current running threads
  //else add to the uthread_queue.
}


void uthread_yield()
{

  //make the u
  uncontext *current_process;
  current_process = (ucontext_t*) malloc(sizeof(ucontext_t);
  current_process->uc_stack.ss_sp = malloc(16384);
  current_process->uc_stack.ss_size = 16384;


  double lowestVal = 0;
  int index = 0;
  double lowest_time = head->timestamp;
  thread_info *list = head;
  thread_info *next_process = malloc(sizeof(thread_info));


  int nodeCount = 0;
  while(list->next != NULL)
  {
    if(list->timestamp < lowest_time)
    {
       next_process = list;

    }
    list = list->next;
  }
  
y
  if()




 ucontext_t *next_process_context = next_process->function_context;
  swapcontext(current_process, next_process);
}


void uthread_exit()
{

}
