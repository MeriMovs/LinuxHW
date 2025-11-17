#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>
#include <semaphore.h>
#include <vector>

class parallel_scheduler {
public:
    parallel_scheduler(int _cap);
    ~parallel_scheduler();
    void run(void (*foo)(int), int arg);

private:
    int cap;
    std::vector<pthread_t> tid_arr;
};


#endif // PARALLEL_SCHEDULER_H