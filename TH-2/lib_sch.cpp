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

void* thread_wrapper(void* void_args) {
// ---------	 magic 		--------- //
    ThreadArgs* args = static_cast<ThreadArgs*>(void_args);
    void (*foo)(int) = args->user_func;
    int arg = args->arg;
// ---------	 </magic> 		--------- //

    sem_wait(&sem);
    foo(arg);
    sem_post(&sem);

    delete args;
    return nullptr;
}


parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
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