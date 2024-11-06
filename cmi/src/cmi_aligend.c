#include "vmi.h"
#include "rasm.h"
#include <cmi.h>
#include <vmi_debug.h>

void __aligned_store64(void *addr, uint64_t val) {
  RASM_SSE_SUPPORT_ENTRY_HOOK();
  ALIGEND_STORE(addr,val);
  RASM_SSE_SUPPORT_EXIT_HOOK();
}
uint64_t __aligned_load64(void *addr) {
  RASM_SSE_SUPPORT_ENTRY_HOOK();
  uint64_t ret=ALIGEND_LOAD(addr);
  RASM_SSE_SUPPORT_EXIT_HOOK();
  return ret;
}

uint8_t *vmi_global_memory_next = 0;

static int cmi_is_initialized = 0;

void __cmi_init() {
  // nop
  // aes_populate_key();
}

void *__cmi_malloc(size_t size) {
  if (!cmi_is_initialized) {
    cmi_is_initialized = 1;
    __cmi_init();
  }
  void *ret = vmi_global_memory_next;
  vmi_global_memory_next += ((size / 8) + 1) * 8; // 64byte aligend
  cmi_dbg(stderr, "%s: ret=%p size=%zx\n", __func__, ret, size);
  return ret;
}

void *__cmi_calloc(size_t num, size_t size) {
  uint64_t *ret = __cmi_malloc(num * size);
  for (int i = 0; (i * 8) < (num * size); i++) {
    __aligned_store64(&(ret[i]), 0);
  }
  return ret;
}

void __cmi_free(void *ptr) {
  // FIXME: ignoref
}

uint8_t __cmi_load8(uint8_t *addr) {
  return (__aligned_load64(addr) >> ((((uint64_t)addr) % 8) * 8)) & 0xff;
}

uint16_t __cmi_load16(uint16_t *addr) {
  return (__aligned_load64(addr) >> ((((uint64_t)addr) % 8) * 8)) & 0xffff;
}

uint32_t __cmi_load32(uint32_t *addr) {
  return (__aligned_load64(addr) >> ((((uint64_t)addr) % 8) * 8)) & 0xffffffff;
}

uint64_t __cmi_load64(uint64_t *addr) { return __aligned_load64(addr); }

static inline uint64_t __cmi_store_mask(void *addr, uint64_t val,
                                        uint64_t mask) {
  uint64_t shl = ((((uint64_t)addr) % 8) * 8);
  uint64_t r1 = __aligned_load64(addr) & (~((uint64_t)(mask << shl)));
  uint64_t r2 = (uint64_t)val << shl;
  return r1 | r2;
}

void __cmi_store8(uint8_t *addr, uint8_t val) {
  __aligned_store64(addr, __cmi_store_mask(addr, val, 0xff));
}

void __cmi_store16(uint16_t *addr, uint16_t val) {
  __aligned_store64(addr, __cmi_store_mask(addr, val, 0xffff));
}

void __cmi_store32(uint32_t *addr, uint32_t val) {
  __aligned_store64(addr, __cmi_store_mask(addr, val, 0xffffffff));
}

void __cmi_store64(uint64_t *addr, uint64_t val) {
  __aligned_store64(addr, val);
}

int __cmi_is_sensitive(void *addr) {
  return 1;
}
