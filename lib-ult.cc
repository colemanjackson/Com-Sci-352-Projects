#include "lib-ult.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <queue>
#include <semaphore.h>
#include <ucontext.h>
#include <unistd.h>



#define gettid()  syscall(SYS_gettid)

using namespace std;

/*
* uthread: a bad threading library using C/C++.
* By Coleman Jackson
* Directions: compile using make
*
*/

typedef struct threadInfo
{
  ucontext_t *context;
  double time_run;

}thread_info;

sem_t sem_thread_lock;
sem_t sem_queue_lock;


//comparator for thread info, necessary for priority queue.
class CompareThreadInfo {
public:
  bool operator()(thread_info* t1, thread_info* t2)
  {
    if (t1->time_run < t2->time_run) return true;
    return false;
  }
};


//priority queue to store the structs
priority_queue<thread_info*, vector<thread_info*>, CompareThreadInfo> pq;

int new_kernel_thread(void *args)
{
  sem_wait(&sem_thread_lock);
  sem_wait(&sem_queue_lock);


  thread_info *shortest_run;
  shortest_run=(thread_info *)malloc(sizeof(thread_info));
  ucontext_t *newContext;
  newContext = (ucontext_t*)malloc(sizeof(ucontext_t));
  shortest_run->context = newContext;

  //perform actual copy of thread_info struct
  *shortest_run = *pq.top();

  //testing next line. delete it
  *newContext = *(shortest_run->context);

  //remove the used thread info from the queue
  pq.pop();

  sem_post(&sem_queue_lock);

  //setcontext(shortest_run->context);
  setcontext(newContext);}

void system_init(int max_number_of_klt)
{ printf("In System init\n");
  sem_init(&sem_thread_lock, 0, max_number_of_klt);
  sem_init(&sem_queue_lock, 0, 1);
  printf("Exiting system init\n");
}

int uthread_create(void (*func)())
{
  //new struct
  thread_info *thread;
  thread=(thread_info *)malloc(sizeof(thread_info));
  ucontext_t *process;
  process = (ucontext_t*) malloc(sizeof(ucontext_t));
  getcontext(process);

    //create a process that will run func when it is called with"setcontext()" or "swapcontext()"
  process->uc_stack.ss_sp = malloc(16384);
  process->uc_stack.ss_size = 16384;
  //IF MALLOC ABOVE EVER RETURNS NULL, YOU FAILED SO RETURN -1
  if(thread == NULL || process == NULL)
  {
    printf("Error, Malloc in uthread_create is NULL\n");
    return -1;

  }

  makecontext(process, func, 0, 0);
  //Add the new process to the queue
  thread->context = process;
  thread->time_run = 0;
  pq.push(thread);
  //create a new kernel thread and run the highest priority thread from the queue
  void *child_stack;
  child_stack=(void *)malloc(16384); child_stack+=16383;
  if(child_stack == NULL)
  {
    return -1;
  }
  
  clone(new_kernel_thread, child_stack, CLONE_VM|CLONE_FILES, NULL);
  return 0;
}


void uthread_yield()
{
  if(pq.empty() == false)
  {
        //get the new struct from the priority queue
    sem_wait(&sem_queue_lock);
    thread_info *shortest = pq.top();
    pq.pop();
    sem_post(&sem_queue_lock);
    ucontext_t *contxt;
    contxt = (ucontext_t*) malloc(sizeof(ucontext_t));
    contxt->uc_stack.ss_sp = malloc(16384);
    contxt->uc_stack.ss_size = 16384;
    
    //add the current context to a struct
    thread_info *current_thread;
    current_thread = (thread_info*)malloc(sizeof(thread_info));
    current_thread->context = contxt;
    int who = RUSAGE_THREAD;
    struct rusage usage;
    int ret;
    ret = getrusage(who, &usage);
    current_thread->time_run = usage.ru_stime.tv_sec;

    //add the current context back to the priority queue
    pq.push(current_thread);

    //set the context to the one that came from the priority queue
    setcontext(shortest->context);
  }
}

void uthread_exit()
{
  sem_post(&sem_thread_lock);
  while(1);
}
