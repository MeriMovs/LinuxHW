#include "lib_sch.h"
#include <iostream>
#include <cstdint>

parallel_scheduler::parallel_scheduler(int _cap) : cap(_cap) {
    buffer = new task[cap];
    threads = new pthread_t[cap];

    for (int i = 0; i < cap; ++i) {
        pthread_create(&threads[i], nullptr, consumer_entry, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    pthread_mutex_lock(&mutex);
    shutdown = 1;
    pthread_cond_broadcast(&cond_fill);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < cap; ++i) {
        pthread_join(threads[i], nullptr);
    }

    delete[] buffer;
    delete[] threads;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_empty);
    pthread_cond_destroy(&cond_fill);
}

void parallel_scheduler::put(const task& t) {
    buffer[fill_ptr] = t;
    fill_ptr = (fill_ptr + 1) % cap;
    count++;
}

task parallel_scheduler::get() {
    task t = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % cap;
    count--;
    return t;
}


// -- нечто непонятное --
void* parallel_scheduler::consumer_entry(void* arg) {
    return ((parallel_scheduler*)arg)->consumer(nullptr);
}

void* parallel_scheduler::consumer(void*) {
    while (true) {
        pthread_mutex_lock(&mutex);

        while (count == 0 && !shutdown)
            pthread_cond_wait(&cond_fill, &mutex);

        if (count == 0 && shutdown) {
            pthread_mutex_unlock(&mutex);
            return nullptr;
        }

        task t = get();
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);

        t.func(t.arg);
    }
}

void parallel_scheduler::run(void (*foo)(int), int arg) {
    task t;
    t.func = (void(*)(void*))foo;
    t.arg = (void*)(uintptr_t)arg;

    pthread_mutex_lock(&mutex);
    while (count == cap)
        pthread_cond_wait(&cond_empty, &mutex);

    put(t);
    pthread_cond_signal(&cond_fill);
    pthread_mutex_unlock(&mutex);
}