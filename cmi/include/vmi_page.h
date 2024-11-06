#ifndef _VMI_PAGE_H_
#define _VMI_PAGE_H_
#include "test_utils.h"
#include <rasm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(VMI_PAGE_USE_INDJMP)
#define vmi_phys_load_64(D) vmi_ZMMx28_indjmp_load((void *)(D))
#define vmi_phys_store_64(D, V) vmi_ZMMx28_indjmp_store((void *)(D), V)
#endif

#if defined(VMI_PAGE_USE_PERM)
#define vmi_phys_load_64(D) vmi_ZMMx28_vpermi2q_oload((void *)(D))
#define vmi_phys_store_64(D, V) vmi_ZMMx28_indjmp_store((void *)(D), V)
#endif

// ------------- RASM --------------
#if defined(VMI_PAGE_USE_RASM_PERM_CMOV)
#define vmi_phys_load_64(D) rasm_perm_load64((void *)(D))
#define vmi_phys_store_64(D, V) rasm_cmov_store64((void *)(D), V)
#endif

#if defined(VMI_PAGE_USE_RASM_PERM_INDJMP)
#define vmi_phys_load_64(D) rasm_perm_load64((void *)(D))
#define vmi_phys_store_64(D, V) rasm_indjmp_store64((void *)(D), V)
#endif

#if defined(VMI_PAGE_USE_RASM_CMOV)
#define vmi_phys_load_64(D) rasm_cmov_load64((void *)(D))
#define vmi_phys_store_64(D, V) rasm_cmov_store64((void *)(D), V)
#endif

#if defined(VMI_PAGE_USE_RASM_INDJMP)
#define vmi_phys_load_64(D) rasm_indjmp_load64((void *)(D))
#define vmi_phys_store_64(D, V) rasm_indjmp_store64((void *)(D), V)
#endif

// -----------------------------------

#ifndef VMI_PAGE_REPLACE_STEP
#define VMI_PAGE_REPLACE_STEP -1
#endif

#if defined(VMI_PAGE_USE_BASE)
extern uint64_t g_base_mem[];
#define vmi_phys_load_64(D) (g_base_mem[((uint64_t)D>>3)])
#define vmi_phys_store_64(D, V) (g_base_mem[((uint64_t)D>>3)] = (V))
#endif

#define PHYS_MEM_SIZE (28 * 64)

// set pinned pages
#ifndef PPN_BASE
#define PPN_BASE 0
#endif

#ifndef VA_OFF_LEN
#define VA_OFF_LEN 6
#endif

#ifndef VA_VPN_LEN
#define VA_VPN_LEN 16
#endif

#define PAGE_SIZE (1 << VA_OFF_LEN)
#define PAGE_COUNT (PHYS_MEM_SIZE / (PAGE_SIZE))
#define PAGE_TABLE_COUNT (1 << VA_VPN_LEN)
#define VIRT_MEM_SIZE (PAGE_TABLE_COUNT * PAGE_SIZE)

#define va_vpn(X) ((((uint64_t)X) >> VA_OFF_LEN) & ((1 << VA_VPN_LEN) - 1))
#define va_off(X) (((uint64_t)X) & ((1 << VA_OFF_LEN) - 1))
#define page_addr(X) ((void *)((uint64_t)(X)*PAGE_SIZE))

#define pte_get_pgn(X) (((uint8_t)X) >> 3)
#define pte_set_pgn(X, Y) X = ((((uint8_t)X) & 0x7) | (((uint8_t)Y) << 3))
#define pte_is_dirty_bit_set(X) ((((uint8_t)X) & 0x1))
#define pte_is_refer_bit_set(X) ((((uint8_t)X) & 0x2))
#define pte_is_valid_bit_set(X) ((((uint8_t)X) & 0x4))
#define pte_set_dirty_bit(X) X = ((uint8_t)X | 0x1)
#define pte_set_refer_bit(X) X = ((uint8_t)X | 0x2)
#define pte_set_valid_bit(X) X = ((uint8_t)X | 0x4)
#define pte_clear_dirty_bit(X) X = ((uint8_t)X & ~0x1)
#define pte_clear_refer_bit(X) X = ((uint8_t)X & ~0x2)
#define pte_clear_valid_bit(X) X = ((uint8_t)X & ~0x4)

typedef struct vmi_context vmi_context_t;
// typedef struct page_table_entry page_table_entry_t;
typedef uint8_t page_table_entry_t;
typedef page_table_entry_t *page_table_t;
typedef struct page_pool page_pool_t;

extern void vmi_init();
extern void *vmi_alloc(size_t);
extern uint64_t vmi_load_64(void *);
extern void vmi_store_64(void *, uint64_t);


static void *virt_to_phys(void *,int);
// Allocate new physical page and return the number. If there are no available
// physical page, swap out pages using replacement algorithm
static uint8_t vmi_get_free_page();

// Swap page in/out into physical address(=pa) the key is virtual page
// number(=vpn),
void swap_page_in(void *pa, uint32_t vpn);
void swap_page_out(void *pa, uint32_t vpn);

#define PGN_NOSWAP ((uint8_t)31)
// union page_table_entry {
struct page_table_entry {
  uint8_t valid_bit : 1;
  uint8_t refer_bit : 1;
  uint8_t dirty_bit : 1;
  uint8_t page_num : 5;
};

struct page_pool {
  uint8_t stack[PAGE_COUNT];
  int top;
};

struct swap_storage {};

struct vmi_stats {
  uint64_t do_measure;
  uint64_t cnt_pgfault;

  uint64_t cnt_swap_in;
  uint64_t time_swap_in;
  uint64_t time_swap_in_prev;

  uint64_t cnt_swap_out;
  uint64_t time_swap_out;
  uint64_t time_swap_out_prev;

  uint64_t cnt_rasm;
  uint64_t time_rasm;
  uint64_t time_rasm_prev;

  uint64_t cnt_trans;
  uint64_t time_trans;
  uint64_t time_trans_prev;
};

struct vmi_context {
  page_table_entry_t page_table[PAGE_TABLE_COUNT];
  int virt_page_table[PAGE_COUNT]; // ppn -> vpn table
  int alloc_page_top;              // Simple grow only allocator
  struct page_pool page_pool;
  // Page replacement: clock algorithm
  uint64_t page_clock_index;
  struct vmi_stats stats;
};

extern vmi_context_t *g_vmi_ctx;
#ifdef VMI_MEASURE_STATS

inline void vmi_init_stats() {
  memset(&g_vmi_ctx->stats, 0, sizeof(struct vmi_stats));
}

static inline void vmi_measure_start() { g_vmi_ctx->stats.do_measure = 1; }
static inline void vmi_measure_stop() { g_vmi_ctx->stats.do_measure = 0; }

static void vmi_print_stats() {
  struct vmi_stats *S = &g_vmi_ctx->stats;
  // clang-format off
  printf("HEADER, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s\n",
         "cnt_trans", "cnt_rasm", "cnt_swap_in", "cnt_swap_out",
         "time_trans", "time_rasm", "time_swap_in", "time_swap_out");
  printf("STATS,  %10ld, %10ld, %10ld, %10ld, %15ld, %15ld, %15ld, %15ld\n",
         S->cnt_trans, S->cnt_rasm, S->cnt_swap_in, S->cnt_swap_out,
         S->time_trans, S->time_rasm, S->time_swap_in, S->time_swap_out);
  // clang-format on
}

static inline void measure_rasm() {
  uint64_t t = get_tick();
  struct vmi_stats *stats = &g_vmi_ctx->stats;
  if (!stats->do_measure)
    return;
  if (stats->time_rasm_prev == 0) {
    stats->cnt_rasm++;
    stats->time_rasm_prev = t;
  } else {
    stats->time_rasm += t - stats->time_rasm_prev;
    stats->time_rasm_prev = 0;
  }
}

static inline void measure_swap_out() {
  uint64_t t = get_tick();
  struct vmi_stats *stats = &g_vmi_ctx->stats;
  if (!stats->do_measure)
    return;
  if (stats->time_swap_out_prev == 0) {
    stats->cnt_swap_out++;
    stats->time_swap_out_prev = t;
  } else {
    stats->time_swap_out += t - stats->time_swap_out_prev;
    stats->time_swap_out_prev = 0;
  }
}
static inline void measure_swap_in() {
  uint64_t t = get_tick();
  struct vmi_stats *stats = &g_vmi_ctx->stats;
  if (!stats->do_measure)
    return;
  if (stats->time_swap_in_prev == 0) {
    stats->cnt_swap_in++;
    stats->time_swap_in_prev = t;
  } else {
    stats->time_swap_in += t - stats->time_swap_in_prev;
    stats->time_swap_in_prev = 0;
  }
}

static inline void measure_trans() {
  uint64_t t = get_tick();
  struct vmi_stats *stats = &g_vmi_ctx->stats;
  if (!stats->do_measure)
    return;
  if (stats->time_trans_prev == 0) {
    stats->cnt_trans++;
    stats->time_trans_prev = t;
  } else {
    stats->time_trans += t - stats->time_trans_prev;
    stats->time_trans_prev = 0;
  }
}

#else
static inline void vmi_measure_start() {}
static inline void vmi_measure_stop() {}
static void vmi_print_stats() {}
static inline void vmi_init_stats() {}
static inline void measure_rasm() {}
static inline void measure_swap_out() {}
static inline void measure_swap_in() {}
static inline void measure_trans() {}

#endif

#endif
