#include <iostream>
#include "lib_sch.h"

void foo1(int arg) {
    int c = 1000;
    while(arg > 0) {
        while(c > 0) {
            c--;
        }
        arg--;
    }
    std::cout << "lala";
}

int main() {
    parallel_scheduler p_sch(5);
    p_sch.run(foo1, 5);
    // p_sch.run(foo1, 5);

    // std::cout << test(5);
    return 0;
}
 