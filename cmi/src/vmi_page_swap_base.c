#include "aes.h"
#include "vmi.h"
#include "vmi_page.h"

#define SWAP_MEM_SIZE (8*(1<<20)) // 8MB

uint8_t* g_swap_mem=NULL;

static void init_swap(){
  g_swap_mem = malloc(SWAP_MEM_SIZE);
  if(!g_swap_mem){
    exit(-1);
  }
}

void swap_page_in(void *pa, uint32_t vpn) {
  if(g_swap_mem==NULL){
    init_swap();
  }
  uint64_t base = ((uint64_t)g_swap_mem) + (((uint64_t)vpn) * PAGE_SIZE);
  for (uint32_t i = 0; i < PAGE_SIZE; i += 8) {
    uint64_t *src = (uint64_t *)(base + i);
    uint64_t *dst = (uint64_t *)(((uint64_t)pa) + i);
    vmi_phys_store_64(dst, *src);
  }
  return;
}

void swap_page_out(void *pa, uint32_t vpn) {
  if(g_swap_mem==NULL){
    init_swap();
  }
  uint64_t base =
      ((uint64_t)g_swap_mem) + (((uint64_t)vpn) * (uint64_t)PAGE_SIZE);
  for (uint32_t i = 0; i < PAGE_SIZE; i += 8) {
    uint64_t *dst = (uint64_t *)(base + i);
    uint64_t *src = (uint64_t *)(((uint64_t)pa) + i);
    *dst= vmi_phys_load_64(src);
  }
  return;
}
