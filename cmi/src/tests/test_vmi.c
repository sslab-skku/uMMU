#include "vmi.h"
#include <stdint.h>
#include <stdio.h>

void test_vmi_obliv_v1();

int main() {
  test_vmi_obliv_v1();
  return 0;
}


void test_vmi_obliv_v1(){
  uint64_t input[8]={
	0x0000000000000000,
	0x1111111111111111,
	0x2222222222222222,
	0x3333333333333333,
	0x4444444444444444,
	0x5555555555555555,
	0x6666666666666666,
	0x7777777777777777,
  };
  uint64_t data;
  asm volatile("mov r10, 0x12345678deadbeaf\n"
			   "vpbroadcastq zmm2, r10\n"
			   "vmovdqu64 zmm3, %0\n"
			   "vmovdqu64 zmm31, %0\n"::"m"(input));
  data = vmi_obliv_load_v1((void*)(64*1+8*3));
  printf("ret: %lx\n", data);
    data = vmi_obliv_load_v1((void*)(64*30+8*3));
  printf("ret: %lx\n", data);

  vmi_obliv_store_v1((void*)(8*3), 0xabcdabcd);
  data = vmi_obliv_load_v1((void*)(8*3));
  printf("ret: %lx\n", data);
}
