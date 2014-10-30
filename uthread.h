#ifndef UTHREAD_h
#define UTHREAD_h
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <semaphore.h>

void system_init(int max_number_of_klt);
int uthread_create(void (*func)());
void uthread_yield();
void uthread_exit();


#endif