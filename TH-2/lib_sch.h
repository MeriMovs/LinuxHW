#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>
#include <queue>

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

    int shutdown = 0;

    std::queue<struct task*> queue;


    pthread_t* threads;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond_newTask = PTHREAD_COND_INITIALIZER;

    void* consumer(void* arg);
    static void* execute(void* arg);
};

#endif