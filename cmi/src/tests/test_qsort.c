#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <test_utils.h>

/* Quick Sort */
int qsort_u32_partition(uint32_t arr[], int low, int high) {
  // choose the pivot
  int pivot = __cmi_load32(&arr[high]);
  uint32_t t1, t2;
  // Index of smaller element and Indicate
  // the right position of pivot found so far
  int i = (low - 1);

  for (int j = low; j <= high; j++) {
    // If current element is smaller than the pivot
    if (__cmi_load32(&arr[j]) < pivot) {
      // Increment index of smaller element
      i++;
      __cmi_swap(arr[i], arr[j], 32);
    }
  }
  __cmi_swap(arr[i + 1], arr[high], 32);
  return (i + 1);
}

void qsort_u32(uint32_t arr[], int low, int high) {
  // when low is less than high
  if (low < high) {
    // pi is the partition return index of pivot

    int pi = qsort_u32_partition(arr, low, high);

    // Recursion Call
    // smaller element than pivot goes left and
    // higher element goes right
    qsort_u32(arr, low, pi - 1);
    qsort_u32(arr, pi + 1, high);
  }
}


int main(int argc, char *argv[]) {
  int len, count, seed;
  uint8_t *cmi_arr;
  uint8_t *buf_arr;
  uint32_t res, h1, h2;
  uint64_t t1, t2, tsum;
  len = (argc > 1) ? atoi(argv[1]) : 1000;
  count = (argc > 2) ? atoi(argv[2]) : 1000000;
  seed = (argc > 3) ? atoi(argv[3]) : 1337;
  // printf("len=%-10d count=%-10d seed=%-10d\n", len, count, seed);

  cmi_arr = __cmi_malloc(len);
  buf_arr = malloc(len);

  // Initialize random array & check
  init_random_arr(buf_arr, len, seed);
  h1 = hash32((char *)buf_arr, len);
  copy_to_cmi(cmi_arr, buf_arr, len);
  h2 = hash32_cmi_arr(cmi_arr, len);
  if (h1 != h2) {
    printf("[ERR] init_arr, h1!=h2\n");
    // return -1;
  }

  // Start Tests
  printf("%25s %13s %10s\n", "TEST", "TICK", "HASH");

  // Quick Sort Test
  tsum = 0;
  for (int i = 0; i < count; i++) {
    // Init random array
    init_random_arr(buf_arr, len, seed + i);
    copy_to_cmi(cmi_arr, buf_arr, len);
    t1 = get_tick();
    qsort_u32((uint32_t *)cmi_arr, 0, len / 4 - 2);
    t2 = get_tick();
    tsum += t2 - t1;
  }
  h1 = hash32_cmi_arr(cmi_arr, len);

  printf("%25s %13lu %10x\n", "qsort_u32", tsum, h1);

  return 0;
}
