#ifndef RSORT_H
#define RSORT_H
#include <cmi.h>
#include <stdint.h>
#include <stdio.h>

// A utility function to get maximum value in arr[]
static uint32_t getMax(uint32_t arr[], uint32_t n) {
  uint32_t mx = __cmi_load32(&arr[0]);
  for (uint32_t i = 1; i < n; i++) {
    uint32_t t = __cmi_load32(&arr[i]);
    if (t > mx)
      mx = t;
  }
  return mx;
}

// A function to do counting sort of arr[] according to
// the digit represented by exp.
static void countSort(uint32_t arr[], uint32_t n, uint32_t exp) {
  uint32_t output[n]; // output array
  uint32_t count[10] = {0};
  int i = 0;

  // Store count of occurrences in count[]
  for (i = 0; i < n; i++) {
    uint32_t t = __cmi_load32(&arr[i]);
    count[(t / exp) % 10]++;
  }

  // Change count[i] so that count[i] now contains actual
  //  position of this digit in output[]
  for (i = 1; i < 10; i++)
    count[i] += count[i - 1];

  // Build the output array
  for (i = n - 1; i >= 0; i--) {
    uint32_t t = __cmi_load32(&arr[i]);
    output[count[(t / exp) % 10] - 1] = t;
    count[(t / exp) % 10]--;
  }

  // Copy the output array to arr[], so that arr[] now
  // contains sorted numbers according to current digit
  for (i = 0; i < n; i++) {
    __cmi_store32(&arr[i], output[i]);
  }
}

// The main function to that sorts arr[] of size n using
// Radix Sort
static void radixsort(uint32_t arr[], uint32_t n) {
  // Find the maximum number to know number of digits
  uint32_t m = getMax(arr, n);

  // Do counting sort for every digit. Note that instead
  // of passing digit number, exp is passed. exp is 10^i
  // where i is current digit number
  for (uint64_t exp = 1; ((uint64_t)m > exp) && ((1 << 31) > exp); exp *= 10)
    countSort(arr, n, exp);
}

#endif
