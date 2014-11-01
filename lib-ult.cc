#include "lib-ult.h"
#include <semaphore.h>
#include <ucontext.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <queue>
#include <sys/time.h>
#include <sys/resource.h>

#define gettid()  syscall(SYS_gettid)

using namespace std;
//compile with g++ -o myprog main.c lib-ult.c -lpthread
// myprog is the name of my project, main.c can be the professor's test file
//10-11 thursday, PEARSON 145

typedef struct threadInfo
{
  ucontext_t *context;
  double time_run;
}thread_info;

//This class tells the priority queue how to compare objects
class CompareThreadInfo {
public:
  bool operator()(thread_info& t1, thread_info& t2)
  {
    if (t1.time_run < t2.time_run) return true;
    return false;
  }
};

//semaphore that makes sure running kernel threads are less than max kernel threads
sem_t thread_lock;
//semaphore that makes sure only one thread is accessing the queue at a time
sem_t queue_lock;
//priority queue to store the structs
priority_queue<thread_info, vector<thread_info>, CompareThreadInfo> pq;

void system_init(int max_number_of_klt)
{
    //initialize this semaphore with the value of max_number_of_klt so that it will keep track of how many threads are currently running andif we are allowed to start anymore running
    sem_init(&thread_lock, 0, max_number_of_klt);
    //initialize this semaphore with the value 1, because we only want 1 thread at a time to access the queue
  sem_init(&queue_lock, 0, 1);
}

int uthread_create(void (*func)())
{
    //create a new struct to add to the queue
  thread_info *thread;
  thread=(thread_info *)malloc(sizeof(thread_info));
    //get the memory you need to store a ucontext_t
  ucontext_t *process;
  process = (ucontext_t*) malloc(sizeof(ucontext_t));
    //create a process that will run func when it is called with"setcontext()" or "swapcontext()"
  process->uc_stack.ss_sp = malloc(16384);
  process->uc_stack.ss_size = 16384;
  makecontext(process, func);
    //IF MALLOC ABOVE EVER RETURNS NULL, YOU FAILED SO RETURN -1

    //Add the new process to the queue
  thread->context = process;
  thread.time_run = 0;
  pq.push(thread);

    //create a new kernel thread and run the highest priority thread from the queue
  void *child_stack;
  child_stack=(void *)malloc(16384); child_stack+=16383;
  clone(kernel_thread, child_stack, CLONE_VM|CLONE_FILES, NULL);

  return 0;
}


void uthread_yield()
{
  if(pq.size()>0)
  {
        //get the new struct from the priority queue
    sem_wait(&queue_lock);
    thread_info *shortest = pq.top();
    pq.pop();
    sem_post(&queue_lock);
        //create a context for the current thread MAY NEED TO CHANGE THE NAME OF PROCESS BECAUSE IT IS USED ABOVE
    ucontext_t *contxt;
    contxt = (ucontext_t*) malloc(sizeof(ucontext_t));
    contxt->uc_stack.ss_sp = malloc(16384);
    contxt->uc_stack.ss_size = 16384;
    getcontext(contxt);

        //add the current context to a struct
    thread_info *current_thread;
    current_thread = (thread_info*)malloc(sizeof(thread_info));
    current_thread->context = contxt;
    int who = RUSAGE_THREAD;
    struct rusage usage;
    int ret;
    ret = getrusage(who, &usage);
    current_thread.time-run = usage->ru_stime->tv_sec;

        //add the current context back to the priority queue
    pq.push(current_thread);

        //set the context to the one that came from the priority queue
    setcontext(shortest->context);
  }
}

void uthread_exit()
{
    //signal that a user thread has stopped and kill the current thread
  sem_post(&thread_lock);
  kill(getid(), SIGKILL);
}

int kernel_thread(void *arg)
{
    //run the user thread if there are enough kernel threads left(currently running - max running)
  sem_wait(&thread_lock);

    //fetch a user thread from the queue
  sem_wait(&queue_lock);
  thread_info *shortest = pq.top();
  pq.pop();
  sem_post(&queue_lock);

    //change context to the thread you want to run
  setcontext(shortest->context);
}