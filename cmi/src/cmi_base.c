#include <cmi.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef CMI_DBG
#define cmi_dbg(...) fprintf(__VA_ARGS__)
#else
#define cmi_dbg(...)                                                           \
  {}
#endif

#define VMI_GLOBAL_MEMORY_SIZE (1024 * 1024)

uint8_t vmi_global_memory[VMI_GLOBAL_MEMORY_SIZE] = {
    0,
};
uint8_t *vmi_global_memory_next = vmi_global_memory;
uint8_t *vmi_global_memory_end = vmi_global_memory + VMI_GLOBAL_MEMORY_SIZE;
static int cmi_is_initialized = 0;

void __cmi_init() {
  // nop
  // aes_populate_key();
}

void *__cmi_malloc(size_t size) {
  void* ret=malloc(size);
  /* if (!cmi_is_initialized) { */
  /*   cmi_is_initialized = 1; */
  /*   __cmi_init(); */
  /* } */
  /* void *ret = vmi_global_memory_next; */
  /* vmi_global_memory_next += ((size / 64) + 1) * 64; // 64byte aligend */
  /* cmi_dbg(stderr, "%s: ret=%p size=%zx\n", __func__, ret, size); */
  return ret;
}

void *__cmi_calloc(size_t num, size_t size) {
  void *ret = __cmi_malloc(num * size);
  for (int i = 0; (i * 8) <= (num * size); i++) {
    __cmi_store64(&((uint64_t *)ret)[i], 0);
  }
  return ret;
}
void __cmi_free(void *ptr) {
  // FIXME: ignoref
}

int __cmi_is_sensitive(void *addr) { return 0; }

uint8_t __cmi_load8(uint8_t *addr) { return *addr; }
uint16_t __cmi_load16(uint16_t *addr) { return *addr; }
uint32_t __cmi_load32(uint32_t *addr) { return *addr; }
uint64_t __cmi_load64(uint64_t *addr) { return *addr; }

void __cmi_store8(uint8_t *addr, uint8_t val) { *addr = val; }
void __cmi_store16(uint16_t *addr, uint16_t val) { *addr = val; }
void __cmi_store32(uint32_t *addr, uint32_t val) { *addr = val; }
void __cmi_store64(uint64_t *addr, uint64_t val) { *addr = val; }
