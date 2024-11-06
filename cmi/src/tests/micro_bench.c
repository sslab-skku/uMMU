/* -*- compile-command: "make -k micro_bench.out && ./micro_bench.out"; -*- */
#include "aes.h"
#include "vmi.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h>

void mbench_8byte_random_access(char *desc, struct vmif *vmif, int len,
                                int count) {
  uint32_t aux;
  uint64_t a, b, t = 0, r;
  load_fn_t load = vmif->load;
  uint64_t *arr = vmif->alloc(len * 8);
  uint64_t *addr = NULL;
  for (int i = 0; i < count; i++) {
    r = random() % len;
    addr = &arr[r];
    a = __rdtscp(&aux);
    load(addr);
    b = __rdtscp(&aux);
    t += b - a;
  }
  printf("%15s avg: %lu\n", desc, t / count);
}

int main() {
  mbench_8byte_random_access("warmup", &vmif_base, 8 * 28, 10000000);
  mbench_8byte_random_access("base", &vmif_base, 8 * 28, 10000000);
  mbench_8byte_random_access("nop", &vmif_nop, 8 * 28, 10000000);
  mbench_8byte_random_access("ZMMx28 indjmp", &vmif_ZMMx28_indjmp, 8 * 28,
                             10000000);
  mbench_8byte_random_access("ZMMx28 vpermi2q", &vmif_ZMMx28_vpermi2q, 8 * 28,
                             10000000);
  mbench_8byte_random_access("ZMMx28 vmov", &vmif_ZMMx28_vmov, 8 * 28, 10000000);

  mbench_8byte_random_access("m4096B", &vmif_m2048B, 2048/8,
                             10000000);
  mbench_8byte_random_access("m2048B", &vmif_m2048B, 2048/8,
                             10000000);
  mbench_8byte_random_access("m1024B", &vmif_m1024B, 1024/8,
                             10000000);

  mbench_8byte_random_access("m512B", &vmif_m1024B, 64,
                             10000000);

  return 0;
}
