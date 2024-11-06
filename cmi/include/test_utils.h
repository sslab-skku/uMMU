#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_
#include <cmi.h>
#include <stdint.h>
#include <x86intrin.h>
#define TEST_LOOP(__COUNT, __STMT, __RES)                                      \
  {                                                                            \
    __RES = UINT64_MAX;                                                        \
    uint32_t __aux;                                                            \
    uint64_t __a, __b, __t;                                                    \
    for (int __i = 0; __i < __COUNT; __i++) {                                  \
      __a = __rdtscp(&__aux);                                                  \
      __STMT;                                                                  \
      __b = __rdtscp(&__aux);                                                  \
      __t = (__b - __a);                                                       \
      __RES = (__RES < __t) ? __RES : __t;                                     \
    }                                                                          \
  }

static inline uint64_t get_tick() {
  uint32_t aux;
  return __rdtscp(&aux);
}

static inline uint64_t get_and_reset_timer() {
  static uint32_t aux;
  static uint64_t prev, cur, ret;
  cur = __rdtscp(&aux);
  ret = cur - prev;
  prev = cur;
  return ret;
}

// h31_hash
static uint32_t hash32(const char *s, size_t len) {
  uint32_t h = 0;
  while (len) {
    h = 31 * h + *s++;
    --len;
  }
  return h;
}

static unsigned int g_seed = 1337;

static inline int fast_rand(void) {
  g_seed = (214013 * g_seed + 2531011);
  return (g_seed >> 16) & 0x7FFF;
}

#define FAST_RAND(L) (fast_rand() % (L))
#define SET_FAST_RAND_SEED(S) (g_seed = S)

#define __cmi_swap(X, Y, S)                                                    \
  {                                                                            \
    uint##S##_t t1 = __cmi_load##S(&X), t2 = __cmi_load##S(&Y);                \
    __cmi_store##S(&X, t2);                                                    \
    __cmi_store##S(&Y, t1);                                                    \
  }

static void init_random_arr(uint8_t *arr, int len, int seed) {
  uint8_t val;
  SET_FAST_RAND_SEED(seed);
  for (int i = 0; i < len; i++) {
    val = FAST_RAND(256);
    arr[i] = val;
  }
}

static void copy_from_cmi(uint8_t *dst, uint8_t *src_cmi, int len) {
  for (int i = 0; i < len; i++) {
    *dst++ = __cmi_load8(src_cmi++);
  }
}

static void copy_to_cmi(uint8_t *dst_cmi, uint8_t *src, int len) {
  for (int i = 0; i < len; i++) {
    __cmi_store8(dst_cmi++, *src++);
  }
}

static inline uint32_t hash32_cmi_arr(uint8_t *src_cmi, int len) {
  uint8_t *buf = malloc(len);
  uint32_t ret;
  copy_from_cmi(buf, src_cmi, len);
  ret = hash32((char *)buf, len);
  free(buf);
  return ret;
}



#endif
