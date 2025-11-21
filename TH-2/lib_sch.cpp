#include "lib_sch.h"
#include <iostream>
#include <cstdint>

parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
    threads = new pthread_t[cap];

    for (int i = 0; i < cap; ++i) {
        pthread_create(&threads[i], nullptr, [](void* arg) -> void* {
        // -- телепорт "нечто непонятного"  --
            return static_cast<parallel_scheduler*>(arg)->consumer(nullptr);
        }, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    pthread_mutex_lock(&mutex);
    shutdown = 1;
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < cap; ++i) {
        pthread_join(threads[i], nullptr);
    }

    delete[] threads;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_newTask);
}

void* parallel_scheduler::consumer(void*) {
    while (true) {
        pthread_mutex_lock(&mutex);

        while (queue.empty() && !shutdown)
            pthread_cond_wait(&cond_newTask, &mutex);

        if (queue.empty() && shutdown) {
            pthread_mutex_unlock(&mutex);
            return nullptr;
        }

        task* t = queue.front();
        queue.pop();

        pthread_mutex_unlock(&mutex);

        t->func(t->arg);
    }
}

void parallel_scheduler::run(void (*foo)(int), int arg) {
    task* t = new task;
    t->func = reinterpret_cast<void(*)(void*)>(foo);
    t->arg = reinterpret_cast<void*>(static_cast<intptr_t>(arg));

    pthread_mutex_lock(&mutex);
    queue.push(t);

    pthread_cond_signal(&cond_newTask);
    pthread_mutex_unlock(&mutex);
}