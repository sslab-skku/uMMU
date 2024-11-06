#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cmi.h>

void __cmi_init(){
  // nop
}

void *__cmi_malloc(size_t size) {
  return malloc(size);
}

void *__cmi_calloc(size_t num, size_t size) {
  return calloc(num,size);
}
void __cmi_free(void *ptr) {
  // FIXME: ignore
}

int __cmi_is_sensitive(void *addr) {
  return 1;
}

uint8_t __cmi_load8(uint8_t *addr) {
  return (*addr) ^ 0x01;
}

uint16_t __cmi_load16(uint16_t *addr) {
  return (*addr) ^ 0x0101;
}

uint32_t __cmi_load32(uint32_t *addr) {
  return (*addr) ^ 0x01010101;
}

uint64_t __cmi_load64(uint64_t *addr) {
  return (*addr) ^ 0x0101010101010101;
}

void __cmi_store8(uint8_t *addr, uint8_t val) {
  *addr = val ^ 0x01;
}

void __cmi_store16(uint16_t *addr, uint16_t val) {
  *addr = val ^ 0x0101;
}

void __cmi_store32(uint32_t *addr, uint32_t val) {
  *addr = val ^ 0x01010101;
}

void __cmi_store64(uint64_t *addr, uint64_t val) {
  *addr = val ^ 0x0101010101010101;
}
