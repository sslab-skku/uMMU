#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

#define BENCH_LOOP(__COUNT, __STMT, __RES)                                     \
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

void test_vpalignr() {
  uint64_t zmm_input[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
  uint64_t zmm_input1[8] = {0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 17};
  uint64_t zmm_input2[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

  asm volatile("vmovdqu64 zmm1, %[in]\n"
               "vpalignr zmm1, zmm1, zmm1, 0x1\n"

               "vmovdqu64 zmm1, %[in]\n"
               "vmovdqu64 zmm2, %[idx]\n"
               "vpermi2q zmm2, zmm1,zmm1\n"
               /* "vpshldq zmm1, zmm1, zmm1, 0x1\n" */
               :
               : [in] "m"(zmm_input), [idx] "m"(zmm_input1));
}

void test_expand_compress_insts() {
  uint64_t zmm_input[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
  uint64_t zmm_input1[8] = {0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3};
  uint64_t zmm_input2[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
  uint64_t read_idx = 0b10000000;
  uint64_t write_idx = 0b00010000;

  asm volatile("mov rax, 0b1\n"
               "kmovq k1, %[read_idx]\n"
               "kmovq k2, %[write_idx]\n"
               "vmovdqu64 zmm1, %[in1]\n"
               "vmovdqu64 zmm2, %[in2]\n"
               :
               : [in1] "m"(zmm_input), [in2] "m"(zmm_input1),
                 [read_idx] "m"(read_idx), [write_idx] "m"(write_idx));

  asm volatile("vpcompressq zmm3%{k1%}, zmm1\n" // read
               "vpexpandq zmm1%{k2%}, zmm2\n"   // write
               :
               :);
}

int main() {
  uint32_t aux;
  uint64_t t1, t2;
  int i = 0;
  uint64_t res = 0;
  test_vpalignr();
  test_expand_compress_insts();

  printf("%ld", t2 - t1);
}

/* Local Variables: */
/* compile-command: "make test_avx.out" */
/* End: */
