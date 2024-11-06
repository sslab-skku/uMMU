#include "aes.h"
#include "test_utils.h"
#include "vmi.h"
#include "vmi_page.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct test_result {
  char *desc;
  uint64_t time;
  uint64_t out_hash;
};

uint64_t xor_test(struct vmif *tar, uint64_t arr[], int len, int count);
uint64_t store_test(struct vmif *tar, uint64_t arr[], int len, int count);
uint64_t swap_test(struct vmif *tar, uint64_t arr[], int len, int count);
void init_arr(struct vmif *target, uint64_t *arr, int len, int seed);
void run_xor_test(char *desc, struct vmif *target, int len, int count);
void run_store_test(char *desc, struct vmif *target, int len, int count);
void run_swap_test(char *desc, struct vmif *target, int len, int count);

int main() {
  int len, count;
  run_xor_test("warmup ", &vmif_ZMMx28_vmov, 100, 10000000);

  len = 28 * 8;
  count = 1000 * 1000;
  printf("---- xor test len:%d, count:%d ----\n", len, count);
  run_xor_test("base", &vmif_base, len, count);
  aes_populate_key();
  run_xor_test("aes", &vmif_aes, len, count);
  vmi_init();
  run_xor_test("vmi page", &vmif_vmi, len, count);

  printf("---- store test len:%d, count:%d ----\n", len, count);
  run_store_test("base", &vmif_base, len, count);
  aes_populate_key();
  run_store_test("aes", &vmif_aes, len, count);
  vmi_init();
  run_store_test("vmi page", &vmif_vmi, len, count);

  printf("---- swap test len:%d, count:%d ----\n", len, count);
  run_swap_test("base", &vmif_base, len, count);
  aes_populate_key();
  run_swap_test("aes", &vmif_aes, len, count);
  vmi_init();
  run_swap_test("vmi page", &vmif_vmi, len, count);

  len = 28 * 8;
  count = 1000 * 1000;
  printf("---- xor test len:%d, count:%d ----\n", len, count);
  run_xor_test("base", &vmif_base, len, count);
  run_xor_test("vmov", &vmif_ZMMx28_vmov, len, count);
  run_xor_test("vpermi2q", &vmif_ZMMx28_vpermi2q, len, count);
  run_xor_test("indjmp", &vmif_ZMMx28_indjmp, len, count);
  aes_populate_key();
  run_xor_test("aes", &vmif_aes, len, count);
  vmi_init();
  run_xor_test("vmi page", &vmif_vmi, len, count);

  printf("---- store test len:%d, count:%d ----\n", len, count);
  run_store_test("base", &vmif_base, len, count);
  run_store_test("vmov", &vmif_ZMMx28_vmov, len, count);
  run_store_test("indjmp", &vmif_ZMMx28_indjmp, len, count);

  len = 14 * 8;
  count = 1000 * 1000;
  printf("---- xor test len:%d, count:%d ----\n", len, count);
  run_xor_test("base", &vmif_base, len, count);
  run_xor_test("linear m1024B", &vmif_m1024B, len, count);
  run_xor_test("vmov", &vmif_ZMMx14_vmov, len, count);
  run_xor_test("vpermi2q", &vmif_ZMMx28_vpermi2q, len, count);
  run_xor_test("vpermi2q ZMMx14", &vmif_ZMMx14_vpermi2q, len, count);
  run_xor_test("vpermq ZMMx14", &vmif_ZMMx14_vpermq, len, count);
  run_xor_test("indjmp", &vmif_ZMMx28_indjmp, len, count);

  printf("---- store test len:%d, count:%d ----\n", len, count);
  run_store_test("base", &vmif_base, len, count);
  run_store_test("vmov", &vmif_ZMMx28_vmov, len, count);
  run_store_test("indjmp", &vmif_ZMMx28_indjmp, len, count);

  vmi_init();
  run_store_test("vmi page", &vmif_vmi, len, count);
}

void init_arr(struct vmif *target, uint64_t *arr, int len, int seed) {
  int val;
  srand(0);
  for (int i = 0; i < len; i++) {
    val = rand() % 1000000;
    target->store(arr + i, val);
  }
}

uint64_t xor_test(struct vmif *tar, uint64_t arr[], int len, int count) {
  uint64_t result = 0, t;
  load_fn_t load = tar->load;
  int i = 0, j = 0;
  while (i++ < count) {
    j = (j * 13 + 7) % len;
    t = load(&arr[j]);
    result ^= t * i;
  }
  return result;
}

uint64_t store_test(struct vmif *tar, uint64_t arr[], int len, int count) {
  uint64_t result = 0;
  store_fn_t store = tar->store;
  int i = 0, j = 0;
  while (i++ < count) {
    j = (j * 13 + 7) % len;
    store(&arr[j], j);
  }
  return result;
}

uint64_t swap_test(struct vmif *tar, uint64_t arr[], int len, int count) {
  uint64_t a = 1, b = 2;
  load_fn_t load = tar->load;
  store_fn_t store = tar->store;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    k = (j * 13 + 7) % len;
    a = load(&arr[j]);
    b = load(&arr[k]);
    store(&arr[j], b);
    store(&arr[k], a);
    j = k;
  }
  return a;
}

void run_xor_test(char *desc, struct vmif *target, int len, int count) {
  uint64_t time, result;
  uint64_t *arr;
  arr = target->alloc(len * 8);
  init_arr(target, arr, len, 0);

  get_and_reset_timer();
  result = xor_test(target, arr, len, count);
  time = get_and_reset_timer();
  result &= 0xffff;
  printf("%-15s %10lu %10lx\n", desc, time, result);
}

void run_store_test(char *desc, struct vmif *target, int len, int count) {
  uint64_t time, result;
  uint64_t *arr;
  arr = target->alloc(len * 8);
  init_arr(target, arr, len, 0);

  get_and_reset_timer();
  store_test(target, arr, len, count);
  time = get_and_reset_timer();
  result = xor_test(target, arr, len, count);
  result &= 0xffffffff;
  printf("%-15s %10lu %10lx\n", desc, time, result);
}

void run_swap_test(char *desc, struct vmif *target, int len, int count) {
  uint64_t time, result;
  uint64_t *arr;
  arr = target->alloc(len * 8);
  init_arr(target, arr, len, 0);

  get_and_reset_timer();
  swap_test(target, arr, len, count);
  time = get_and_reset_timer();
  result = xor_test(target, arr, len, count);
  result &= 0xffffffff;
  printf("%-15s %10lu %10lx\n", desc, time, result);
}
