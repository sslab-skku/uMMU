#include "vmi_page.h"
#include "vmi.h"
#include "vmi_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vmi_context_t *g_vmi_ctx;
uint64_t g_base_mem[256]; // replaces the rasm 

void vmi_init() {
  g_vmi_ctx = calloc(1, sizeof(vmi_context_t));
  if (g_vmi_ctx == NULL) {
    fprintf(stderr, "vmi_init calloc failed\n");
    exit(-1);
  }
  // init page pool, push PPN_BASE~PAGE_COUNT
  struct page_pool *pool = &g_vmi_ctx->page_pool;
  for (int i = PPN_BASE; i < PAGE_COUNT; i++) {
    pool->stack[pool->top++] = i;
  }
  g_vmi_ctx->alloc_page_top = 0;
}

static inline page_table_entry_t vmi_lookup_pgt(uint32_t n) {
  page_table_t pgt = g_vmi_ctx->page_table;
  page_table_entry_t ret = pgt[n];
#ifdef VMI_OBLIV_PAGE_LOOKUP
  const int JMP = 64;
  for (int i = (n % JMP); i < g_vmi_ctx->alloc_page_top; i += JMP) {
    page_table_entry_t pte = pgt[i];
    if (n == i && (ret != pte)) {
      ret = pte;
    }
  }
#else
  ret = pgt[n];
#endif
  return ret;
}

uint64_t vmi_load_64(void *va) {
  /* dbg_printf("vpn = %ld,virt_to_phys(%p)  = %p\n", va_vpn(va), va, */
  /*            virt_to_phys(va)); */
  measure_trans();
  void *upa = virt_to_phys(va, 0);
  measure_rasm();
  uint64_t ret = vmi_phys_load_64(upa);
  measure_rasm();
  measure_trans();
  return ret;
}

void vmi_store_64(void *va, uint64_t val) {
  measure_trans();
  void *upa = virt_to_phys(va, 1);
  measure_rasm();
  vmi_phys_store_64(upa, val);
  measure_rasm();
  measure_trans();
}

void *vmi_alloc(size_t size) {
  int pg_cnt = ((size - 1) / PAGE_SIZE) + 1;
  int *top = &g_vmi_ctx->alloc_page_top;
  int old_top = *top;
  page_table_t pgt = g_vmi_ctx->page_table;
  if (old_top + pg_cnt > PAGE_TABLE_COUNT) {
    fprintf(stderr, "%s: Failed old_top: %d, pg_cnt: %d\n", __func__, old_top,
            pg_cnt);
    return 0;
  }
  for (int i = 0; i < pg_cnt; i++) {
    page_table_entry_t pte = pgt[(*top)];
    pte_clear_valid_bit(pte);
    pte_clear_refer_bit(pte);
    pte_clear_dirty_bit(pte);
    pte_set_pgn(pte, PGN_NOSWAP);
    pgt[(*top)] = pte;
    (*top)++;
    // pte_clear_valid_bit(pte);
    //  pte->valid_bit = 0;
    //  pte->refer_bit = 0;
  }
  return page_addr(old_top);
}

static inline uint8_t vmi_get_free_page() {
  vmi_context_t *ctx = g_vmi_ctx;
  page_pool_t *pool = &ctx->page_pool;
  // if there are no available pages
  if (pool->top == 0) {
    // free unused page
    uint64_t *pci = &ctx->page_clock_index;
    page_table_t pgt = ctx->page_table;
    while (1) {
      uint32_t vpn = *pci;
      int top = ctx->alloc_page_top;
      *pci = ((*pci + top) + VMI_PAGE_REPLACE_STEP) % top;
      page_table_entry_t pte = pgt[vpn];
      /* if (pte->valid_bit == 0) */
      if (!pte_is_valid_bit_set(pte))
        continue;
#ifdef VMI_PAGE_USE_CLOCK
      if (pte_is_refer_bit_set(pte)) {
        pte_clear_refer_bit(pte); // pte->refer_bit = 0;
        pgt[vpn]=pte; // update
        continue;
      }
#endif
      int pgn = pte_get_pgn(pte); // pte->page_num;
      // swap out
      pte_clear_valid_bit(pte); // pte->valid_bit = 0;
      dbg_printf("%s: swap out vpn,pgn: %d,%d\n", __func__, vpn, pgn);
      int do_swap_out = pte_is_dirty_bit_set(pte);
#ifdef DISABLE_DIRTY_BIT_OPT
      do_swap_out=1; // Always swap out the selected page
#endif
      if (do_swap_out) {
        measure_swap_out();
        swap_page_out(page_addr(pgn), vpn);
        measure_swap_out();
      }
      // add to page pool
      // dbg_printf("%s: added %d\n", __func__, pte->page_num);
      pool->stack[pool->top++] = pte_get_pgn(pte); // ->page_num;
      pgt[vpn] = pte;
      break;
    }
  }
  // pop page
  // dbg_printf("%s: popped %d\n", __func__, pool->stack[pool->top]);
  return pool->stack[--pool->top];
}

static inline void *virt_to_phys(void *va, int is_write) {
  uint32_t vpn = va_vpn(va);
  page_table_t pgt = g_vmi_ctx->page_table;
  page_table_entry_t pte;

  pte = vmi_lookup_pgt(vpn);

  // if the page does not exist, swap in the page
  if (!pte_is_valid_bit_set(pte)) {
    dbg_printf("Page %d does not exists! swap in\n", vpn);
    uint8_t pgn = vmi_get_free_page();
    if (pte_get_pgn(pte) != PGN_NOSWAP) {
      measure_swap_in();
      swap_page_in(page_addr(pgn), vpn);
      measure_swap_in();
    }
    // pte = pgt[vpn];
    pte_set_pgn(pte, pgn);
    pte_clear_dirty_bit(pte);
    pte_set_valid_bit(pte);
    // pgt[vpn].page_num = pgn;
    // pgt[vpn].valid_bit = 1;
  }
  if (is_write) {
    pte_set_dirty_bit(pte);
  }
  pte_set_refer_bit(pte); //  pgt[vpn].refer_bit = 1;
  // update
  pgt[vpn] = pte;
  int pgn = pte_get_pgn(pte); //.page_num;
  return (void *)((pgn << VA_OFF_LEN) | va_off(va));
}
