#include "oram.h"
#include "vmi.h"
#include "vmi_page.h"



#ifndef ORAM_Z
#define ORAM_Z 4
#endif
#ifndef ORAM_L
#define ORAM_L 10
#endif

#define SWAP_MEM_SIZE ((ORAM_BLOCK_SIZE*ORAM_Z)*(1<<ORAM_L))

uint8_t* g_swap_mem = NULL;
static oram_ctx *g_oram_ctx = NULL;

static void init_swap(){
  g_oram_ctx = oram_alloc_ctx(ORAM_Z, ORAM_L);
  g_swap_mem = malloc(SWAP_MEM_SIZE);
}

void swap_page_in(void *pa, uint32_t vpn) {
  if (g_swap_mem == NULL) {
    init_swap();
  }
  uint8_t buf[PAGE_SIZE];
  oram_read(g_oram_ctx, buf, vpn*PAGE_SIZE, PAGE_SIZE);

  uint64_t base = ((uint64_t)g_swap_mem) + (((uint64_t)vpn) * PAGE_SIZE);
#if 1
  base = buf;
#endif
  for (uint32_t i = 0; i < PAGE_SIZE; i += 8) {
    uint64_t *src = (uint64_t *)(base + i);
    uint64_t *dst = (uint64_t *)(((uint64_t)pa) + i);
    vmi_phys_store_64(dst, *src);
  }
  return;
}

void swap_page_out(void *pa, uint32_t vpn) {
  if (g_swap_mem == NULL) {
    init_swap();
  }
  uint64_t base =
      ((uint64_t)g_swap_mem) + (((uint64_t)vpn) * (uint64_t)PAGE_SIZE);
  uint8_t buf[PAGE_SIZE];

#if 1
  base=(uint64_t)buf;
#endif

  for (uint32_t i = 0; i < PAGE_SIZE; i += 8) {
    uint64_t *dst = (uint64_t *)(base + i);
    uint64_t *src = (uint64_t *)(((uint64_t)pa) + i);
    *dst = vmi_phys_load_64(src);
  }

  oram_write(g_oram_ctx, (void*)(vpn*PAGE_SIZE), buf, PAGE_SIZE);
  return;
}
