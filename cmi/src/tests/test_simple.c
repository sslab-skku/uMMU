#include <cmi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  uint64_t *arr = NULL;
  int ret = 0;
  int len;
  len = (argc > 1) ? atoi(argv[1]) : 192;
  printf("Simple Test len = %d\n", len);
  arr = __cmi_malloc(len * sizeof(uint64_t));
  for (uint64_t i = 0; i < len; i++) {
    __cmi_store64(&arr[i], i);
  }
  for (uint64_t i = 0; i < len; i++) {
    uint64_t t = __cmi_load64(&arr[i]);
    printf("%3lx ", t);
    if ((i + 1) % 8 == 0) {
      printf("\n");
    }
  }
  printf("4byte test\n");
  uint32_t *arr32 = (uint32_t*)arr;
  for (uint64_t i = 0; i < len; i++) {
    __cmi_store32(&arr32[i], i);
  }
  for (uint64_t i = 0; i < len; i++) {
    uint32_t t = __cmi_load32(&arr32[i]);
    printf("%3x ", t);
    if ((i + 1) % 8 == 0) {
      printf("\n");
    }
  }

  printf("1byte test\n");
  uint8_t *arr8 = (uint8_t*)arr;
  for (uint64_t i = 0; i < len; i++) {
    __cmi_store8(&arr8[i], i);
  }
  for (uint64_t i = 0; i < len; i++) {
    uint8_t t = __cmi_load8(&arr8[i]);
    printf("%3x ", t);
    if ((i + 1) % 8 == 0) {
      printf("\n");
    }
  }
  return ret;
}
