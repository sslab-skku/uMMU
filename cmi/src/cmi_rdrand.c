#include <cmi.h>
#include <stdio.h>
#include <stdlib.h>
#include "rdrand.h"
/***********  RDRAND IMPLEMENTATION  *************/

#define XOR_MASK (0x0102030405060708)

#define SHADOW_MEM_SIZE (1ULL << 26)
#define SHADOW_MASK SHADOW_MEM_SIZE-1

uint64_t *vmi_salt_memory = NULL;

uint64_t xorshift128p_seed[2];

// Function to seed the xorshift128+ generator
void xorshift128p_init() {
    uint64_t seed1 = rdrand();
    uint64_t seed2 = rdrand();
    while(seed1 == 0 || seed2 == 0 || seed1 == seed2) {
        seed1 = rdrand();
        seed2 = rdrand();
    }
    xorshift128p_seed[0] = seed1;
    xorshift128p_seed[1] = seed2;
}

// Function to generate a random 64-bit integer using xorshift128+
uint64_t xorshift128p() {
    uint64_t t = xorshift128p_seed[0];
    uint64_t const s = xorshift128p_seed[1];
    xorshift128p_seed[0] = s;
    t ^= t << 23; // a
    t ^= t >> 18;		// b -- Again, the shifts and the multipliers are tunable
	  t ^= s ^ (s >> 5);
    xorshift128p_seed[1] = t; // b, c
    return t + s; // New state
}

void __aligned_store64(void *addr, uint64_t val) {
  addr = (void *)((((uint64_t)addr) >> 3) << 3); // 64bit aligend pointer
  uint64_t salt_offset = ((uint64_t)addr) >> 3 & SHADOW_MASK; // offset to shadow memory
  uint64_t salt = rdrand();
  // uint64_t salt = xorshift128p();
  vmi_salt_memory[salt_offset] = salt;
  (*(uint64_t *)addr) = val ^ salt;
}

uint64_t __aligned_load64(void *addr) {
  addr = (void *)((((uint64_t)addr) >> 3) << 3); // 64bit aligend pointer
  uint64_t salt_offset = ((uint64_t)addr) >> 3 & SHADOW_MASK; // offset to shadow memory
  return (*(uint64_t *)addr) ^ vmi_salt_memory[salt_offset];
}

static int cmi_is_initialized = 0;

void __cmi_init() {
  vmi_salt_memory = (uint64_t *)malloc(SHADOW_MEM_SIZE * sizeof(uint64_t));
  xorshift128p_init();
}

void *__cmi_malloc(size_t size) {
  if (!cmi_is_initialized) {
    cmi_is_initialized = 1;
    __cmi_init();
  }
  return malloc(size);
}


/****************      END     *****************/

/*************** DO NOT CHANGE  ***************/
void *__cmi_calloc(size_t num, size_t size) {
  void *ret = __cmi_malloc(num * size);
  for (int i = 0; (i * 8) <= (num * size); i++) {
    __aligned_store64(&((uint64_t *)ret)[i], 0);
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
