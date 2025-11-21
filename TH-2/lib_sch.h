// #ifndef PARALLEL_SCHEDULER_H
// #define PARALLEL_SCHEDULER_H

// #include <pthread.h>

// struct task;

// class parallel_scheduler {
// public:
//     parallel_scheduler(int _cap);
//     ~parallel_scheduler();
//     void run(void (*foo)(int), int arg);

//     void put(struct task task);
//     struct task get();

//     void* producer(void *arg);
//     void* consumer();

//     static void* consumer_wrapper(void *arg);





// private:
//     int cap;
//     int fill_ptr = 0;
//     int use_ptr = 0;
//     int count = 0;
//     int shutdown = 0;

//     struct task *buffer;

//     pthread_t *tid_arr;

//     int* thread_ids;

//     // std::vector<pthread_t> tid_arr;
// };


// #endif // PARALLEL_SCHEDULER_H



