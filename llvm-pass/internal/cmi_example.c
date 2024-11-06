#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cmi.h"

// #define cmi_dbg(...) fprintf(__VA_ARGS__)

#define cmi_dbg(...) {}

#define VMI_GLOBAL_MEMORY_SIZE (1024 * 1024)

uint8_t vmi_global_memory[VMI_GLOBAL_MEMORY_SIZE] = {
    0,
};
uint8_t *vmi_global_memory_next = vmi_global_memory;
uint8_t *vmi_global_memory_end = vmi_global_memory + VMI_GLOBAL_MEMORY_SIZE;

struct {
  int load_count;
  int store_count;
} cmi_log = {
    0,
};

void __cmi_init(){
  // nop
}

void *__cmi_malloc(size_t size) {
  void *ret = vmi_global_memory_next;
  vmi_global_memory_next += size;
  cmi_dbg(stderr, "%s: ret=%p size=%zx\n", __func__, ret, size);
  return ret;
}

void *__cmi_calloc(size_t num, size_t size) {
  void *ret = __cmi_malloc(num * size);
  memset(ret, 0x01, num * size);
  return ret;
}
void __cmi_free(void *ptr) {
  // FIXME: ignore
}

int __cmi_is_sensitive(void *addr) {
  return ((void *)vmi_global_memory <= addr) &&
         (addr < (void *)vmi_global_memory_end);
}

uint8_t __cmi_load8(uint8_t *addr) {
  cmi_dbg(stderr, "load_count: %d\n", cmi_log.load_count++);
  return (*addr) ^ 0x01;
}

uint16_t __cmi_load16(uint16_t *addr) {
  cmi_dbg(stderr, "load_count: %d\n", cmi_log.load_count++);
  return (*addr) ^ 0x0101;
}

uint32_t __cmi_load32(uint32_t *addr) {
  cmi_dbg(stderr, "load_count: %d\n", cmi_log.load_count++);
  return (*addr) ^ 0x01010101;
}

uint64_t __cmi_load64(uint64_t *addr) {
  cmi_dbg(stderr, "load_count: %d\n", cmi_log.load_count++);
  return (*addr) ^ 0x0101010101010101;
}

void __cmi_store8(uint8_t *addr, uint8_t val) {
  cmi_dbg(stderr, "store_count: %d\n", cmi_log.store_count++);
  *addr = val ^ 0x01;
}

void __cmi_store16(uint16_t *addr, uint16_t val) {
  cmi_dbg(stderr, "store_count: %d\n", cmi_log.store_count++);
  *addr = val ^ 0x0101;
}

void __cmi_store32(uint32_t *addr, uint32_t val) {
  cmi_dbg(stderr, "store_count: %d\n", cmi_log.store_count++);
  *addr = val ^ 0x01010101;
}

void __cmi_store64(uint64_t *addr, uint64_t val) {
  cmi_dbg(stderr, "store_count: %d\n", cmi_log.store_count++);
  *addr = val ^ 0x0101010101010101;
}
