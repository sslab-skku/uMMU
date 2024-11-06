#ifndef _CMI_H_
#define _CMI_H_
#include <stddef.h>
#include <stdint.h>

typedef unsigned __int128 uint128_t;
// typedef uint64_t uint128_t[2];

// clang-format off
void cmi_init() __attribute__((used));
void *cmi_malloc(size_t size) __attribute__((used));
void *cmi_calloc(size_t num, size_t size) __attribute__((used));
void cmi_free(void* ptr) __attribute__((used));
int cmi_is_sensitive(void *addr) __attribute__((always_inline)) __attribute__((used));

uint8_t cmi_load8(uint8_t *addr) __attribute__((always_inline)) __attribute__((used));
uint16_t cmi_load16(uint16_t *addr) __attribute__((always_inline)) __attribute__((used));
uint32_t cmi_load32(uint32_t *addr) __attribute__((always_inline)) __attribute__((used));
uint64_t cmi_load64(uint64_t *addr) __attribute__((always_inline)) __attribute__((used));
uint128_t cmi_load128(uint128_t *addr) __attribute__((always_inline)) __attribute__((used));

void cmi_store8(uint8_t *addr, uint8_t val) __attribute__((always_inline)) __attribute__((used));
void cmi_store16(uint16_t *addr, uint16_t val) __attribute__((always_inline)) __attribute__((used));
void cmi_store32(uint32_t *addr, uint32_t val) __attribute__((always_inline)) __attribute__((used));
void cmi_store64(uint64_t *addr, uint64_t val) __attribute__((always_inline)) __attribute__((used));
void cmi_store128(uint128_t *addr, uint128_t val) __attribute__((always_inline)) __attribute__((used));


// Implementations
extern void __cmi_init() __attribute__((used));
extern void *__cmi_malloc(size_t size) __attribute__((used));
extern void *__cmi_calloc(size_t num, size_t size) __attribute__((used));
extern void __cmi_free(void* ptr) __attribute__((used));
extern int __cmi_is_sensitive(void *addr) __attribute__((used));

extern uint8_t __cmi_load8(uint8_t *addr) __attribute__((used));
extern uint16_t __cmi_load16(uint16_t *addr) __attribute__((used));
extern uint32_t __cmi_load32(uint32_t *addr) __attribute__((used));
extern uint64_t __cmi_load64(uint64_t *addr) __attribute__((used));

extern void __cmi_store8(uint8_t *addr, uint8_t val) __attribute__((used));
extern void __cmi_store16(uint16_t *addr, uint16_t val) __attribute__((used));
extern void __cmi_store32(uint32_t *addr, uint32_t val) __attribute__((used));
extern void __cmi_store64(uint64_t *addr, uint64_t val) __attribute__((used));

// clang-format on

#endif
