#include "rdrand.h"
#include "vmi.h"
#include "vmi_page.h"

#define SHADOW_MEM_SIZE (1ULL << 26)
#define SHADOW_MASK SHADOW_MEM_SIZE-1

uint64_t *vmi_salt_memory = NULL;

uint64_t xorshift128p_seed[2];

// TODO change the rdrand macro
#define RDRAND_SWAP_LOAD64(X) (*X)
#define RDRAND_SWAP_STORE64(X,Y) *X=Y;

// uint8_t* g_swap_mem=NULL;

void rdrand_store64(void *addr, uint64_t val) {
  addr = (void *)((((uint64_t)addr) >> 3) << 3); // 64bit aligend pointer
  uint64_t salt_offset = ((uint64_t)addr) >> 3 & SHADOW_MASK; // offset to shadow memory
  uint64_t salt = rdrand();
  // uint64_t salt = xorshift128p();
  vmi_salt_memory[salt_offset] = salt;
  (*(uint64_t *)addr) = val ^ salt;
}

uint64_t rdrand_load64(void *addr) {
  addr = (void *)((((uint64_t)addr) >> 3) << 3); // 64bit aligend pointer
  uint64_t salt_offset = ((uint64_t)addr) >> 3 & SHADOW_MASK; // offset to shadow memory
  return (*(uint64_t *)addr) ^ vmi_salt_memory[salt_offset];
}

static void init_swap(){
  // g_swap_mem = malloc(SWAP_MEM_SIZE);
  vmi_salt_memory = (uint64_t *)malloc(SHADOW_MEM_SIZE * sizeof(uint64_t));
  if(!vmi_salt_memory){
    exit(-1);
  }
}

void swap_page_in(void *pa, uint32_t vpn) {
  if(vmi_salt_memory==NULL){
    init_swap();
  }
  uint64_t base = ((uint64_t)vmi_salt_memory) + (((uint64_t)vpn) * PAGE_SIZE);
  for (uint32_t i = 0; i < PAGE_SIZE; i += 8) {
    uint64_t *src = (uint64_t *)(base + i);
    uint64_t *dst = (uint64_t *)(((uint64_t)pa) + i);
    vmi_phys_store_64(dst, rdrand_load64(src));
    //vmi_phys_store_64(dst, *src);
  }
  return;
}
void swap_page_out(void *pa, uint32_t vpn) {
  if(vmi_salt_memory==NULL){
    init_swap();
  }
  uint64_t base =
      ((uint64_t)vmi_salt_memory) + (((uint64_t)vpn) * (uint64_t)PAGE_SIZE);
  for (uint32_t i = 0; i < PAGE_SIZE; i += 8) {
    uint64_t *dst = (uint64_t *)(base + i);
    uint64_t *src = (uint64_t *)(((uint64_t)pa) + i);
    rdrand_store64(dst, vmi_phys_load_64(src));
  }
  return;
}
