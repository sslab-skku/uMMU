#include "vmi.h"
#include <cmi.h>
#include <stdio.h>
#include "rasm.h"

#ifdef CMI_DBG
#define cmi_dbg(...) fprintf(__VA_ARGS__)
#else
#define cmi_dbg(...)                                                           \
  {}
#endif

#define VMI_GLOBAL_MEMORY_SIZE (64 * 27)

#if defined(CMI_ZMM_USE_VMOV)
#define ___aligned_store64 vmi_ZMMx28_vmov_ostore
#define ___aligned_load64 vmi_ZMMx28_vmov_oload

#elif defined(CMI_ZMM_USE_PERM)
#define ___aligned_store64 vmi_ZMMx28_vmov_ostore
#define ___aligned_load64 vmi_ZMMx28_vpermi2q_oload

#elif defined(CMI_ZMM_USE_PERM_CMOV)
#define ___aligned_store64 vmi_ZMMx28_vmov_ostore
#define ___aligned_load64 vmi_ZMMx28_vpermi2q_oload


#elif defined(CMI_ZMM_USE_PERM_INDJMP)
#define ___aligned_store64 vmi_ZMMx28_indjmp_store
#define ___aligned_load64 vmi_ZMMx28_vpermi2q_oload


#elif defined(CMI_ZMM_USE_PERM_V2)
#define ___aligned_store64 zmm7_30_perm_store
#define ___aligned_load64 zmm7_30_perm_load

#elif defined(CMI_ZMM_USE_INDJMP)
/* #define ___aligned_store64 rasm_indjmp_store64 */
/* #define ___aligned_load64 rasm_indjmp_load64 */

#define ___aligned_store64 vmi_ZMMx28_indjmp_store
#define ___aligned_load64 vmi_ZMMx28_indjmp_load


#else
#define ___aligned_store64 vmi_ZMMx28_vmov_ostore
#define ___aligned_load64 vmi_ZMMx28_indjmp_load
#endif

void __aligned_store64(void *addr, uint64_t val) {
  ___aligned_store64(addr,val);
}
uint64_t __aligned_load64(void *addr) {
  return ___aligned_load64(addr);
}


uint8_t *vmi_global_memory_next = 0;
// uint8_t *vmi_global_memory_end = (uint8_t)VMI_GLOBAL_MEMORY_SIZE;
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

int __cmi_is_sensitive(void *addr) {
  return 1;
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
#if 0
  if (r1 != 0 || r2 != 0) {
    cmi_dbg(stderr, "mk:%016lx --------\n", mask);
    cmi_dbg(stderr, "sl:%016lx\n",~((uint64_t)(mask << shl)));
    cmi_dbg(stderr, "r1:%016lx\n", r1);
    cmi_dbg(stderr, "r2:%016lx\n", r2);
  }
#endif

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
