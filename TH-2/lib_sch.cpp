#include "lib_sch.h"
#include <iostream>
#include <cstdint>

parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
    threads = new pthread_t[cap];

    for (int i = 0; i < cap; ++i) {
        pthread_create(&threads[i], nullptr, worker, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    pthread_mutex_lock(&mutex);
    shutdown = 1;
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond_new_task);

    for (int i = 0; i < cap; ++i) {
        pthread_join(threads[i], nullptr);
    }

    delete[] threads;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_new_task);
}

void* parallel_scheduler::worker(void* arg) {
    parallel_scheduler* scheduler = (parallel_scheduler*)(arg);
    
    while (true) {
        pthread_mutex_lock(&scheduler->mutex);

        while (scheduler->queue.empty() && !scheduler->shutdown)
            pthread_cond_wait(&scheduler->cond_new_task, &scheduler->mutex);

        if (scheduler->queue.empty() && scheduler->shutdown) {
            pthread_mutex_unlock(&scheduler->mutex);
            return nullptr;
        }

        task* t = scheduler->queue.front();
        scheduler->queue.pop();

        pthread_mutex_unlock(&scheduler->mutex);

        t->func();

        delete t;
    }
}

void parallel_scheduler::run(void (*foo)(int), int arg) {
    task* t = new task;
    t->func = [foo, arg]() { foo(arg); };

    pthread_mutex_lock(&mutex);
    queue.push(t);

    pthread_cond_signal(&cond_new_task);
    pthread_mutex_unlock(&mutex);
}