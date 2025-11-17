/*
Create a C++ library (shared object) that implements a simple thread pool

All the functions given to the “run” method are enqueued and the thread pool should execute them as soon as any of its threads gets time.

Next, another executable application is implemented to demonstrate the usage of the thread pool library.
The demonstration application should enqueue various functions for the execution (more than the capacity).
The module “parallel_scheduler” should be compiled as a shared library (parallel_scheduler.so) and should be linked with the demo application.
*/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include "lib_sch.h"


sem_t sem;

struct ThreadArgs {
    void (*user_func)(int);
    int arg;
};

// ---------	 magic 		--------- //
void* thread_wrapper(void* void_args) {
    ThreadArgs* args = static_cast<ThreadArgs*>(void_args);
    void (*foo)(int) = args->user_func;
    int arg = args->arg;

    sem_wait(&sem);

    foo(arg);

    sem_post(&sem);

    delete args;
    return nullptr;
}

// ---------	 </magic> 		--------- //



parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
	// tid = new pthread_t[cap];
	sem_init(&sem, 0, cap);
}

parallel_scheduler::~parallel_scheduler() {	
	for(auto& tid : tid_arr) {
        pthread_join(tid, nullptr);
    }
	sem_destroy(&sem); 
}


void parallel_scheduler::run(void (*foo)(int), int arg) {
    ThreadArgs* args = new ThreadArgs{foo, arg};
    pthread_t tid;
    pthread_create(&tid, nullptr, &thread_wrapper, args);
    tid_arr.push_back(tid);
}


// int test(int in){
//         return in+1;
// }
