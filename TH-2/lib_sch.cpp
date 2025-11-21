#include "lib_sch.h"
#include <iostream>
#include <cstdint>
#include <queue>

parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
    threads = new pthread_t[cap];

    for (int i = 0; i < cap; ++i) {
        pthread_create(&threads[i], nullptr, execute, this);
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

void parallel_scheduler::put(const task& t) {
    struct task* temp = new struct task;
    temp->func = t.func;
    temp->arg = t.arg;
    queue.push(temp);
    count++;
}

task parallel_scheduler::get() {
    struct  task* t = queue.front();
    queue.pop();
    count--;
    return *t;
}


// -- нечто непонятное --
void* parallel_scheduler::execute(void* arg) {
    return ((parallel_scheduler*)arg)->consumer(nullptr);
}

void* parallel_scheduler::consumer(void*) {
    while (true) {
        pthread_mutex_lock(&mutex);

        while (count == 0 && !shutdown)
            pthread_cond_wait(&cond_newTask, &mutex);

        if (count == 0 && shutdown) {
            pthread_mutex_unlock(&mutex);
            return nullptr;
        }

        task t = get();
        pthread_mutex_unlock(&mutex);

        t.func(t.arg);
    }
}

void parallel_scheduler::run(void (*foo)(int), int arg) {
    task t;
    t.func = (void(*)(void*))foo;
    t.arg = (void*)(uintptr_t)arg;

    pthread_mutex_lock(&mutex);
    put(t);
    pthread_cond_signal(&cond_newTask);
    pthread_mutex_unlock(&mutex);
}