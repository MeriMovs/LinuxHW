#include "lib_sch.h"
#include <iostream>
#include <cstdint>

parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
    threads = new pthread_t[cap];

    for (int i = 0; i < cap; ++i) {
        pthread_create(&threads[i], nullptr, [](void* arg) -> void* {
            return static_cast<parallel_scheduler*>(arg)->worker(nullptr);
        }, this);
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

void* parallel_scheduler::worker(void*) {
    task* t;
    while (true) {
        pthread_mutex_lock(&mutex);

        while (queue.empty() && !shutdown)
            pthread_cond_wait(&cond_new_task, &mutex);

        if (queue.empty() && shutdown) {
            pthread_mutex_unlock(&mutex);
            return nullptr;
        }

        t = queue.front();
        queue.pop();

        pthread_mutex_unlock(&mutex);

        t->func(t->arg);

    }
    delete t;
}

void parallel_scheduler::run(void (*foo)(int), int arg) {
    task* t = new task;
    t->func = reinterpret_cast<void(*)(void*)>(foo);
    t->arg = reinterpret_cast<void*>(static_cast<intptr_t>(arg));

    pthread_mutex_lock(&mutex);
    queue.push(t);

    pthread_cond_signal(&cond_new_task);
    pthread_mutex_unlock(&mutex);
}