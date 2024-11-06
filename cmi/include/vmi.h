#ifndef _VMI_H_
#define _VMI_H_

#include <stdint.h>
#include <stdlib.h>
#include "vmi_page.h"
#include "aes.h"



typedef uint64_t (*load_fn_t)(void *);
typedef void (*store_fn_t)(void *, uint64_t);
typedef void *(*alloc_fn_t)(uint64_t);

// alloc
static void *aligned_alloc_1024(size_t size) {
  return aligned_alloc(1024, 1024 * ((size / 1024) + 1));
}
static void *aligned_alloc_2048(size_t size) {
  return aligned_alloc(2048, 2048 * ((size / 2048) + 1));
}
static void *aligned_alloc_4096(size_t size) {
  return aligned_alloc(4096, 4096 * ((size / 2048) + 1));
}
static void *alloc_at_null(size_t size) { return NULL; }

static uint64_t base_load(void *addr) { return *(uint64_t *)addr; }
static void base_store(void *addr, uint64_t val) { *(uint64_t *)addr = val; }
static uint64_t nop_load(void *addr) { return 0; }
static void nop_store(void *addr, uint64_t val) {}

extern uint64_t zmm7_30_perm_load(void *);
extern uint64_t vmi_ZMMx28_vmov_oload(void *);
extern uint64_t vmi_ZMMx14_vmov_oload(void *);
extern uint64_t vmi_ZMMx28_indjmp_load(void *);
extern uint64_t vmi_ZMMx28_vpermi2q_oload(void *);
extern uint64_t vmi_ZMMx14_vpermi2q_oload(void *);
extern uint64_t vmi_ZMMx14_vpermq_oload(void *);

extern void zmm7_30_perm_store(void *, uint64_t);
extern void vmi_ZMMx28_vmov_ostore(void *, uint64_t);
extern void vmi_ZMMx28_indjmp_store(void *, uint64_t);
extern void vmi_ZMMx28_vpermi2q_ostore(void *, uint64_t); /* TODO */


static void vmi_simple_store(void *addr, uint64_t val) {
  *(uint64_t *)addr = val;
}

extern uint64_t vmi_m2048B_oload(void *);
extern uint64_t vmi_m1024B_oload(void *);
extern uint64_t vmi_m512B_oload(void *);

extern void vmi_m2048B_ostore(void *, uint64_t);
extern void vmi_m1024B_ostore(void *, uint64_t);
extern void vmi_m512B_ostore(void *, uint64_t);

static uint64_t vmi_m4096B_oload(void *addr) {
  uint64_t offset = (uint64_t)addr & (4096 - 1);
  uint64_t ret1 = vmi_m1024B_oload(addr + 1024 * 0);
  uint64_t ret2 = vmi_m1024B_oload(addr + 1024 * 1);
  uint64_t ret3 = vmi_m1024B_oload(addr + 1024 * 2);
  uint64_t ret4 = vmi_m1024B_oload(addr + 1024 * 3);
  if (offset < 1024 * 1)
    return ret1;
  else if (offset < 1024 * 2)
    return ret2;
  else if (offset < 1024 * 3)
    return ret3;
  else if (offset < 1024 * 4)
    return ret4;
  return 0;
}

// VMI interface
struct vmif {
  char *name;
  load_fn_t load;
  store_fn_t store;
  alloc_fn_t alloc;
};

// clang-format off
static struct vmif vmif_base = {
  .load = base_load,
  .store = base_store,
  .alloc = aligned_alloc_1024};

static struct vmif vmif_nop = {
  .load = nop_load,
  .store = nop_store,
  .alloc = aligned_alloc_1024};

static struct vmif vmif_m4096B = {
  .load = vmi_m4096B_oload,
  .store = base_store,			/* TODO */
  .alloc = aligned_alloc_4096};

static struct vmif vmif_m2048B = {
  .load = vmi_m2048B_oload,
  .store = base_store,			/* TODO */
  .alloc = aligned_alloc_2048};

static struct vmif vmif_m1024B = {
  .load = vmi_m1024B_oload,
  .store = base_store,			/* TODO */
  .alloc = aligned_alloc_1024};

static struct vmif vmif_m512B = {
  .load = vmi_m512B_oload,
  .store = base_store,			/* TODO */
  .alloc = aligned_alloc_1024};

static struct vmif vmif_ZMMx28_vpermi2q = {
  .load = vmi_ZMMx28_vpermi2q_oload,
  .store = vmi_ZMMx28_indjmp_store, /* TODO */
  .alloc = alloc_at_null};

static struct vmif vmif_ZMMx14_vpermi2q = {
  .load = vmi_ZMMx14_vpermi2q_oload,
  .store = vmi_ZMMx28_indjmp_store, /* TODO */
  .alloc = alloc_at_null};

static struct vmif vmif_ZMMx14_vpermq = {
  .load = vmi_ZMMx14_vpermq_oload,
  .store = vmi_ZMMx28_indjmp_store, /* TODO */
  .alloc = alloc_at_null};



static struct vmif vmif_ZMMx28_vmov = {
  .load = vmi_ZMMx28_vmov_oload,
  .store = vmi_ZMMx28_vmov_ostore,
  .alloc = alloc_at_null};

static struct vmif vmif_ZMMx14_vmov = {
  .load = vmi_ZMMx14_vmov_oload,
  .store = vmi_ZMMx28_vmov_ostore, /* TODO */
  .alloc = alloc_at_null};

static struct vmif vmif_ZMMx28_indjmp = {
  .load = vmi_ZMMx28_indjmp_load,
  .store = vmi_ZMMx28_indjmp_store,
  .alloc = alloc_at_null};

static struct vmif vmif_vmi = {
  .load = vmi_load_64,
  .store = vmi_store_64,
  .alloc = vmi_alloc};

static struct vmif vmif_aes = {
  .load = aes_load,
  .store = aes_store,
  .alloc = aligned_alloc_1024};
// clang-format on

#endif
