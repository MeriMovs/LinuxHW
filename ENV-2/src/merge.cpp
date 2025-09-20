#include "merge.h"
#include <cmath>

void merge(std::vector<ComplexNum>& arr, int left, int mid, int right) {
    int s1 = mid - left + 1;
    int s2 = right - mid;

    std::vector<ComplexNum> lArr(s1);
    std::vector<ComplexNum> rArr(s2);

    for (int i = 0; i < s1; i++) { lArr[i] = arr[left + i];}
    for (int j = 0; j < s2; j++) { rArr[j] = arr[mid + 1 + j]; }

    int i = 0, j = 0;
    int k = left;

    while (i < s1 && j < s2) {
        double absLeft = lArr[i].absoluteValue();
        double absRight = rArr[j].absoluteValue();
        
        if (absLeft < absRight || (absLeft == absRight && lArr[i].getReal() <= rArr[j].getReal())) {
            arr[k++] = lArr[i++];
        } else {
            arr[k++] = rArr[j++];
        }
    }

    while (i < s1) { arr[k++] = lArr[i++]; }
    while (j < s2) { arr[k++] = rArr[j++]; }
}

void mergeSort(std::vector<ComplexNum>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}