#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cmi.h"

#define CMI_ADDR_TAG ((uint64_t)1 << 63)

#define CMI_ADDR_WRAP_TAG(X) ((void *)((CMI_ADDR_TAG) | ((uint64_t)X)))
#define CMI_ADDR_UNWRAP_TAG(X) ((void *)((~CMI_ADDR_TAG) & ((uint64_t)X)))
#define CMI_ADDR_HAS_TAG(X) (((uint64_t)X) & CMI_ADDR_TAG)

void cmi_init() { return __cmi_init(); }

void *cmi_malloc(size_t size) { return CMI_ADDR_WRAP_TAG(__cmi_malloc(size)); }

void *cmi_calloc(size_t num, size_t size) {
  return CMI_ADDR_WRAP_TAG(__cmi_calloc(num, size));
}

void cmi_free(void *ptr) {
  if (CMI_ADDR_HAS_TAG(ptr)) {
    //__cmi_free(CMI_ADDR_UNWRAP_TAG(ptr));
  } else {
    free(ptr);
  }
}

int cmi_is_sensitive(void *addr) { return __cmi_is_sensitive(addr); }

uint8_t cmi_load8(uint8_t *addr) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    return __cmi_load8((uint8_t *)CMI_ADDR_UNWRAP_TAG(addr));
  } else {
    return (*addr);
  }
}

uint16_t cmi_load16(uint16_t *addr) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    return __cmi_load16((uint16_t *)CMI_ADDR_UNWRAP_TAG(addr));
  } else {
    return (*addr);
  }
}

uint32_t cmi_load32(uint32_t *addr) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    return __cmi_load32((uint32_t *)CMI_ADDR_UNWRAP_TAG(addr));
  } else {
    return (*addr);
  }
}

uint64_t cmi_load64(uint64_t *addr) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    return __cmi_load64((uint64_t *)CMI_ADDR_UNWRAP_TAG(addr));
  } else {
    return (*addr);
  }
}

uint128_t cmi_load128(uint128_t *addr) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    uint64_t *unwrap_addr = (uint64_t *)CMI_ADDR_UNWRAP_TAG(addr);
    uint128_t ret = 0;
    ret = __cmi_load64(unwrap_addr);
    ret <<= 64;
    ret |= __cmi_load64(unwrap_addr + 1);
    return ret;
  } else {
    return (*addr);
  }
}

void cmi_store8(uint8_t *addr, uint8_t val) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    __cmi_store8((uint8_t *)CMI_ADDR_UNWRAP_TAG(addr), val);
  } else {
    *addr = val;
  }
}

void cmi_store16(uint16_t *addr, uint16_t val) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    __cmi_store16((uint16_t *)CMI_ADDR_UNWRAP_TAG(addr), val);
  } else {
    *addr = val;
  }
}

void cmi_store32(uint32_t *addr, uint32_t val) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    __cmi_store32((uint32_t *)CMI_ADDR_UNWRAP_TAG(addr), val);
  } else {
    *addr = val;
  }
}

void cmi_store64(uint64_t *addr, uint64_t val) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    __cmi_store64((uint64_t *)CMI_ADDR_UNWRAP_TAG(addr), val);
  } else {
    *addr = val;
  }
}

void cmi_store128(uint128_t *addr, uint128_t val) {
  if (CMI_ADDR_HAS_TAG(addr)) {
    uint64_t *unwrap_addr = (uint64_t *)CMI_ADDR_UNWRAP_TAG(addr);
    __cmi_store64(unwrap_addr, val);
    __cmi_store64(unwrap_addr + 1, (val >> 64));
  } else {
    *addr = val;
  }
}
