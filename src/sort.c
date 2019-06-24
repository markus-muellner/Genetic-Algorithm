#include "sort.h"

void merge(int *arr, int *l, int lLength, int *r, int rLength,
           double *const scrs) {
  /*
   Merges two subarrays into one target array in an ordered way.

   @param arr: Target array
   @param l: Left subarray
   @param lLength: Number of elements in the left subarray
   @param r: Right subarray
   @param rLength: Number of elements in the right subarray
   @param scrs: Array with values that are used for comparison

   @return void: Array is modified directly
   */

  int i = 0, j = 0, k = 0;

  while (i < lLength && j < rLength) {
    if (scrs[l[i]] < scrs[r[j]])
      arr[k++] = l[i++];
    else
      arr[k++] = r[j++];
  }
  while (i < lLength)
    arr[k++] = l[i++];
  while (j < rLength)
    arr[k++] = r[j++];
}

void mergeSort(int *arr, double *const scrs, int length) {
  /*
   Sorts the given array arr accordig to the values in scrs.

   @param arr: Array that will be sorted in place
   @param scrs: Array with values that are used for comparison
   @param length: Number of items in arr and scrs

   @return void: Array is modified directly
   */

  int mid, i;
  int *l, *r;

  /* If less than two elements are left => return */
  if (length < 2) {
    return;
  }

  /* Determine mid of array */
  mid = length / 2;

  /* Allocate left and right subarrays */
  l = malloc(mid * sizeof(int));
  r = malloc((length - mid) * sizeof(int));

  /* Initialize left and right subarrays */
  for (i = 0; i < mid; i++)
    l[i] = arr[i];
  for (i = mid; i < length; i++)
    r[i - mid] = arr[i];

  /* Recursively sort left and right subarrays */
  mergeSort(l, scrs, mid);
  mergeSort(r, scrs, length - mid);

  /* Merge both subarrays back into arr */
  merge(arr, l, mid, r, length - mid, scrs);
  free(l);
  free(r);
}
