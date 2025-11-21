#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include "lib_sch.h"
#include <unistd.h>
#include <cstdint>

struct task {
    void (*func)(void*);
    void *arg;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
    buffer = new struct task[cap];
    tid_arr = new pthread_t[cap];
    thread_ids = new int[cap];

	for(int i = 0; i < cap; ++i) {
        thread_ids[i] = i;
        pthread_create(&tid_arr[i], nullptr, consumer_wrapper, &thread_ids[i]);
    }
	pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&empty, nullptr);
	pthread_cond_init(&fill, nullptr);
}

parallel_scheduler::~parallel_scheduler() {	
	for(int i = 0; i < cap; ++i) {
        pthread_join(tid_arr[i], nullptr);
    }
	pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty);
	pthread_cond_destroy(&fill);

    delete[] tid_arr;
    delete[] buffer;
    delete[] thread_ids;
}


void parallel_scheduler::put(struct task task) {
    buffer[fill_ptr] = task;
    fill_ptr = (fill_ptr + 1) % cap;
    count++;
}

struct task parallel_scheduler::get() {
    struct task task = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % cap;
    count--;
    return task;
}

void* parallel_scheduler::producer(void *arg) {
    struct task *arg_foo = (struct task*)arg;
    
    pthread_mutex_lock(&mutex);
    
    while (count == cap) {
        pthread_cond_wait(&empty, &mutex);
    }
    
    put(*arg_foo);
    
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

// -- непонятно что и для чего -- //
void* parallel_scheduler::consumer_wrapper(void *arg) {
    parallel_scheduler *scheduler = (parallel_scheduler*)arg;
    return scheduler->consumer();
}
//   ------------------------   //


void* parallel_scheduler::consumer() {
    while (true) {
        pthread_mutex_lock(&mutex);
        
        while (count == 0 && !shutdown) {
            pthread_cond_wait(&fill, &mutex);
        }
        
        if (count == 0 && shutdown) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        struct task task = get();
        
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        
        task.func(task.arg);
    }

    return nullptr;
}


void parallel_scheduler::run(void (*foo)(int), int arg) {
     struct task* t = new struct task;
     t->func = (void (*)(void*))foo;
     t->arg = (void*)(uintptr_t)arg;

    producer(t);
}
