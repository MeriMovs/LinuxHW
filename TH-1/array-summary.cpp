#include <iostream>
#include <ctime>
#include <random>

class P_class {
public:
    pthread_t tid;
    int* arr;
    int start;
    int end;
    int res;
};

void* t_sum(void* arg){
    P_class* td = (P_class*) arg;

    td->res = 0;
    for(int i = td->start; i < td->end; i++) {
        td->res += td->arr[i];
    }

    return NULL;
}

void sum(int n, int th_count) {
    int arr[n];// = {1, 2, 3, 4, 5, 6 ,7, 8, 9, 10};

    // - - -  random  - - -
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 10);
    
    for (int i = 0; i < n; i++) { arr[i] = dis(gen); }
    // - - - - - - - - - - - - - - -


    P_class th_datas[th_count];
    int chank = n / th_count;

    std::clock_t clock_start = std::clock();

    for (int i = 0; i < th_count; ++i) {
        th_datas[i].arr = arr;
        th_datas[i].start = i*chank;
        th_datas[i].end = (i+1)*chank;
        int threadCreated = pthread_create(&th_datas[i].tid, NULL, t_sum, (void*) &th_datas[i]); 
        if(threadCreated != 0){
            std::cerr << "Could not create new thread" << std::endl;
            return;
        }
        
    }

    long int th_sum = 0;
    for (int i = 0; i < th_count; ++i) {
        pthread_join(th_datas[i].tid, NULL);
        th_sum += th_datas[i].res;
    }
    std::clock_t clock_end = std::clock();
    double duration = 1000.0 * (clock_end - clock_start) / CLOCKS_PER_SEC;

    std::cout << "sum with threads took " << duration << " ms" << std::endl;
    std::cout << th_sum <<  "\n";
    

    // ---  without -------------

    clock_start = std::clock();
    long int sum = 0; 
    for (int i = 0; i < n; i++) { 
        sum += arr[i];
    }
    clock_end = std::clock();
    duration = 1000.0 * (clock_end - clock_start) / CLOCKS_PER_SEC;

    std::cout << "sum without threads took " << duration << " ms" << std::endl;
    std::cout << sum << "\n";



}

int main(int argc, char** argv) {
    if (argc < 3) {
        perror("Wrong args");
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);  //array size
    int m = atoi(argv[2]);  //cout of threads


    sum(n, m);
    return 0;
}
