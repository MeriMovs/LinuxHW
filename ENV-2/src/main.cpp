#include "../include/complexNum.h"
#include "../include/merge.h"
#include <iostream>
#include <vector>
#include <iomanip>

//--------  by AI))  ---------// 

int main() {
    std::vector<ComplexNum> arr = {
        ComplexNum(3, 4),   // |3 + 4i| = 5
        ComplexNum(1, 1),   // |1 + 1i| ≈ 1.414
        ComplexNum(0, 2),   // |0 + 2i| = 2
        ComplexNum(5, 12),  // |5 + 12i| = 13
        ComplexNum(2, -3)   // |2 - 3i| ≈ 3.606
    };

    std::cout << "Before sorting:\n";
    for (const auto& c : arr) {
        std::cout << c << " (magnitude: " << std::fixed << std::setprecision(3) 
                  << c.absoluteValue() << ")\n";
    }

    // Perform merge sort
    mergeSort(arr, 0, arr.size() - 1);

    std::cout << "\nAfter sorting by magnitude:\n";
    for (const auto& c : arr) {
        std::cout << c << " (magnitude: " << std::fixed << std::setprecision(3) 
                  << c.absoluteValue() << ")\n";
    }

    return 0;
}