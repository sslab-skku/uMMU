#ifndef CMI_APP_COMMON_H
#define CMI_APP_COMMON_H

#define CMI_APP

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAS_X86INTRIN_H
#include <x86intrin.h>
#endif

#define CMI_ADDR_TAG ((uint64_t)1 << 63)

#define CMI_ADDR_WRAP_TAG(X) ((void *)((CMI_ADDR_TAG) | ((uint64_t)X)))
#define CMI_ADDR_UNWRAP_TAG(X) ((void *)((~CMI_ADDR_TAG) & ((uint64_t)X)))
#define CMI_ADDR_HAS_TAG(X) (((uint64_t)X) & CMI_ADDR_TAG)

/* Annotation */
static void *__attribute__((noinline)) anno_secret_malloc(size_t size) {
  return malloc(size);
}

static void *__attribute__((noinline))
anno_secret_calloc(size_t num, size_t size) {
  return calloc(num, size);
}

/* Replace Memory Access */
// clang-format off
static void *__attribute__((noinline)) anno_cmi_malloc(size_t size)             { return malloc(size);}
static void *__attribute__((noinline)) anno_cmi_calloc(size_t num, size_t size) { return calloc(num, size);}
static __attribute__((noinline)) uint8_t anno_cmi_load8(uint8_t *addr)       { return *addr;}
static __attribute__((noinline)) uint16_t anno_cmi_load16(uint16_t *addr)    { return *addr;}
static __attribute__((noinline)) uint32_t anno_cmi_load32(uint32_t *addr)    { return *addr;}
static __attribute__((noinline)) uint64_t anno_cmi_load64(uint64_t *addr)    { return *addr;}
// static uint128_t cmi_load128(uint128_t *addr) __attribute__((noinline)) { return;}

static __attribute__((noinline)) void anno_cmi_store8(uint8_t *addr, uint8_t val)    {*addr = val;}
static __attribute__((noinline)) void anno_cmi_store16(uint16_t *addr, uint16_t val) {*addr = val;}
static __attribute__((noinline)) void anno_cmi_store32(uint32_t *addr, uint32_t val) {*addr = val;}
static __attribute__((noinline)) void anno_cmi_store64(uint64_t *addr, uint64_t val) {*addr = val;}
// static void cmi_store128(uint128_t *addr, uint128_t val) __attribute__((noinline)) { return;}


#ifdef X___DISALBE
#define x___calloc calloc
#define x___malloc malloc
#define x___ld64(X) (*X)
#define x___ld32(X) (*X)
#define x___ld16(X) (*X)
#define x___ld8(X)  (*X)

#define x___st64(X,Y) *X=Y
#define x___st32(X,Y) *X=Y
#define x___st16(X,Y) *X=Y
#define x___st8(X,Y)  *X=Y
#endif
#define x___calloc anno_cmi_calloc
#define x___malloc anno_cmi_malloc
#define x___ld64(X) ((typeof((*X)))(anno_cmi_load64((uint64_t*)(X))))
#define x___ld32(X) ((typeof((*X)))(anno_cmi_load32((uint32_t*)(X))))
#define x___ld16(X) ((typeof((*X)))(anno_cmi_load16((uint16_t*)(X))))
#define x___ld8(X)  ((typeof((*X)))(anno_cmi_load8((uint8_t*)(X))))

#define x___st64(X,Y) anno_cmi_store64((uint64_t*)(X),(uint64_t)Y)
#define x___st32(X,Y) anno_cmi_store32((uint32_t*)(X),(uint32_t)Y)
#define x___st16(X,Y) anno_cmi_store16((uint16_t*)(X),(uint16_t)Y)
#define x___st8(X,Y)  anno_cmi_store8((uint8_t*)(X),(uint8_t)Y)
#else


//#define _St64(X) (typeof((*X)))(anno)

// clang-format on

#define MAKE_SENSITIVE(PTR_VAR, LEN)                                           \
  {                                                                            \
    void *PREV_PTR_VAR = PTR_VAR;                                              \
    PTR_VAR = anno_secret_malloc(LEN);                                         \
    memcpy(PTR_VAR, PREV_PTR_VAR, LEN);                                        \
  }

#define CMI_CONVERT_TO_SENSITIVE_MPI(X)                                        \
  {                                                                            \
    void *__p = (X).p;                                                         \
    size_t __sz = (X).n * 8;                                                     \
    (X).p = anno_secret_malloc(__sz);                                          \
    memcpy((X).p, __p, __sz);                                                  \
  }

#define CMI_INIT_SECURE_MPI(X)                                                 \
  {                                                                            \
    (X).p = anno_secret_malloc(8);                                             \
    (X).n = 1;                                                                 \
  }
#define CMI_INIT_SECURE_MPI_TAINT(X, Y)                                        \
  {                                                                            \
    if (CMI_ADDR_HAS_TAG(Y)) {                                                 \
      (X).p = anno_secret_malloc(8);                                           \
      (X).n = 1;                                                               \
    }                                                                          \
  }

#define CMI_IS_SENSITIVE(X) (CMI_ADDR_HAS_TAG(X))

static inline uint64_t get_and_reset_timer() {
  static uint32_t aux;
  static uint64_t prev, cur, ret;
#ifdef HAS_X86INTRIN_H
  cur = __rdtscp(&aux);
#else
  uint64_t rax, rdx;
  asm volatile("rdtscp\n" : "=a"(rax), "=d"(rdx), "=c"(aux) : :);
  cur = (rdx << 32) + rax;
#endif
  ret = cur - prev;
  prev = cur;
  return ret;
}

static inline uint64_t print_time_stamp(const char *desc, uint64_t prev) {
  uint32_t aux;
  uint64_t cur = 0;
  uint64_t start = 0;

#ifdef HAS_X86INTRIN_H
  cur = __rdtscp(&aux);
#else
  uint64_t rax, rdx;
  asm volatile("rdtscp\n" : "=a"(rax), "=d"(rdx), "=c"(aux) : :);
  cur = (rdx << 32) + rax;
#endif

  start = (prev == 0) ? cur : prev;

  fprintf(stderr, "\n[%c]<%s>%20lu: +%10lu\n", (start == cur) ? 'S' : 'E', desc,
          start, (cur - start));
  return cur;
}

#endif
