#include <cmi.h>
#include <vmi_page.h>
#include <stdio.h>

// VMI Page Implementation
#define __aligned_store64 vmi_store_64
#define __aligned_load64 vmi_load_64

#if 0
void __aligned_store64(void *addr, uint64_t val) {
  vmi_store_64(addr,val);
}
uint64_t __aligned_load64(void *addr) {
  return vmi_load_64(addr);
}
#endif


#if defined(CMI_DBG)
#define cmi_dbg(...) fprintf(__VA_ARGS__)
#else
#define cmi_dbg(...)                                                           \
  {}
#endif

#define VMI_GLOBAL_MEMORY_SIZE (1024 * 1024)

static int cmi_is_initialized = 0;

void __cmi_init() {
  vmi_init();
}

void *__cmi_malloc(size_t size) {
  if (!cmi_is_initialized) {
    cmi_is_initialized = 1;
    __cmi_init();
  }
  cmi_dbg(stderr,"alloc %d\n",size);
  return vmi_alloc(size);
}

void *__cmi_calloc(size_t num, size_t size) {
  void *ret = __cmi_malloc(num * size);
  for (int i = 0; (i * 8) < (num * size); i++) {
    __aligned_store64(&((uint64_t *)ret)[i], 0);
  }
  return ret;
}
void __cmi_free(void *ptr) {
  // FIXME: ignoref
}

int __cmi_is_sensitive(void *addr) {
  return 0;
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

void __cmi_store64(uint64_t *addr, uint64_t val) { __aligned_store64(addr, val); }