#include "vmi.h"
#include <stdint.h>
#include <stdio.h>

uint64_t swap_test(uint64_t arr[], int len, int count) {
  uint64_t a = 1, b = 2;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    k = (j * 2 + 7) % len;
    a = vmi_load_64(&arr[j]);
    b = vmi_load_64(&arr[k]);
    vmi_store_64(&arr[j], b);
    vmi_store_64(&arr[k], a);
  }
  return a;
}

int simple_test() {
  printf("=== Simple Test ===\n");
  vmi_init();
  uint64_t *arr = vmi_alloc(16 * sizeof(uint64_t));
  for (int i = 0; i < 16; i++) {
    vmi_store_64(&arr[i], i * i);
  }
  for (int i = 0; i < 16; i++) {
    uint64_t v = vmi_load_64(&arr[i]);
    if (v != i * i) {
      printf("vmi_load_64(&arr[%d]) is %ld (Expected %d)\n", i, v, i * i);
      printf("Failed\n");
      return -1;
    }
  }
  printf("PASSED\n");
  return 0;
}

int swap_page_copy_test() {
  printf("=== swap page copy test ===\n");
  vmi_init();
  uint64_t *arr = 0; // vmi_alloc(16 * sizeof(uint64_t));
  for (int i = 0; i < 8; i++) {
    vmi_phys_store_64(&arr[i], i);
  }
  swap_page_out(arr, 0);
  // clear vmi memory
  for (int i = 0; i < 8; i++) {
    vmi_phys_store_64(&arr[i], 0);
  }
  swap_page_in(arr, 0);
  for (int i = 0; i < 8; i++) {
    uint64_t v = vmi_phys_load_64(&arr[i]);
    if (v != i) {
      printf("Failed\n");
      return -1;
    }
  }
  printf("PASSED\n");
  return 0;
}

int main() {
  simple_test();
  swap_page_copy_test();
}
