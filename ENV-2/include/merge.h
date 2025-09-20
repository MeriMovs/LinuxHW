#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include "complexNum.h"
#include <vector>

void merge(std::vector<ComplexNum>& arr, int left, int mid, int right);
void mergeSort(std::vector<ComplexNum>& arr, int left, int right);

#endif