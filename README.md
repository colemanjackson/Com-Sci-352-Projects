












void system_init(int max_number_of_klt)

This function is called before any other uthread library functions can be called. You may use it to initialize the uthread system (for example, data structures). Parameter max_num_of_klt specifies the maximum number of kernel threads that can be used to run the user-level threads created by this library. Argument max_number_of_klt should be no less than 1. 
 
(2) int uthread_create(void (*func)())

This function creates a new user-level thread which runs func(). It returns 0 if succeeds, or -1 otherwise (if the library is unable to create the thread due to certain reason such as lack of resource). 

(3) void uthread_yield()

The calling thread requests to yield the kernel thread that it is currently running on to one of other user threads which has run for the shortest period of time. If no any other thread has shorter running time than this calling thread, the calling thread should proceed to run on the kernel thread. 
   
(4) void uthread_exit()

The calling user-level thread ends its execution. You can assume that user application code always call this function before a user thread ends. 
void system_init(int max_number_of_klt)

This function is called before any other uthread library functions can be called. You may use it to initialize the uthread system (for example, data structures). Parameter max_num_of_klt specifies the maximum number of kernel threads that can be used to run the user-level threads created by this library. Argument max_number_of_klt should be no less than 1. 
 
(2) int uthread_create(void (*func)())

This function creates a new user-level thread which runs func(). It returns 0 if succeeds, or -1 otherwise (if the library is unable to create the thread due to certain reason such as lack of resource). 

(3) void uthread_yield()

The calling thread requests to yield the kernel thread that it is currently running on to one of other user threads which has run for the shortest period of time. If no any other thread has shorter running time than this calling thread, the calling thread should proceed to run on the kernel thread. 
   
(4) void uthread_exit()

The calling user-level thread ends its execution. You can assume that user application code always call this function before a user thread ends. 
