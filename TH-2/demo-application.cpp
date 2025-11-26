#include <iostream>
#include "lib_sch.h"

int i = 0, j = 0, m = 0;

void foo1(int arg) {
    while(arg > 0) {
        i++;
        arg--;
    }
    std::cout << i << "\n" ;
}
void foo2(int arg) {
    while(arg > 0) {
        j++;
        arg--;
    }
    std::cout << j  << "\n" ;
}
void foo3(int arg) {
    while(arg > 0) {
        m++;
        arg--;
    }
    std::cout << m << "\n" ;
}

int main() {
    parallel_scheduler p_sch(2);

    p_sch.run(foo1, 50000);
    p_sch.run(foo2, 50000);
    p_sch.run(foo3, 50000);

    return 0; 
}