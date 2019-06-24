#ifndef sort_h
#define sort_h

#include <stdlib.h>

void merge(int *arr, int *l, int leftCount, int *r, int rightCount,
           double *const scrs);
void mergeSort(int *a, double *const scrs, int n);

#endif /* sort_h */
