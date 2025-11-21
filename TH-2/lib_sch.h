#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>

struct task {
    void (*func)(void*);
    void* arg;
};

class parallel_scheduler {
public:
    parallel_scheduler(int _cap);
    ~parallel_scheduler();

    void run(void (*foo)(int), int arg);

private:
    int cap;
    int fill_ptr = 0;
    int use_ptr = 0;
    int count = 0;
    int shutdown = 0;

    struct task* buffer;
    pthread_t* threads;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
    pthread_cond_t cond_fill = PTHREAD_COND_INITIALIZER;

    void put(const struct task& t);
    struct task get();

    void* consumer(void* arg);
    static void* consumer_entry(void* arg);
};

#endif