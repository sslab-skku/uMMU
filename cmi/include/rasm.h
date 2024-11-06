#ifndef RASM_H
#define RASM_H
#include <stdint.h>



extern uint64_t rasm_cmov_load64(void *);
extern void rasm_cmov_store64(void *, uint64_t);

extern uint64_t rasm_perm_load64(void *);

extern uint64_t rasm_indjmp_load64(void *);
extern void rasm_indjmp_store64(void *, uint64_t);


extern uint64_t rasm_dirjmp_load64(void *);
extern void rasm_dirjmp_store64(void *, uint64_t);


extern void rasm_sse_support_copy_from_shelter_regs();
extern void rasm_sse_support_copy_to_shelter_regs();

extern void rasm_sse2_support_copy_from_shelter_regs();
extern void rasm_sse2_support_copy_to_shelter_regs();

#define RASM_ENTRY_HOOKS
#define RASM_EXIT_HOOKS

#if !defined(RASM_SSE_SUPPORT) && !defined(RASM_SSE2_SUPPORT)
#ifndef RASM_NOSSE_SUPPORT
#define RASM_NOSSE_SUPPORT
#endif
#endif

#if defined(RASM_NOSSE_SUPPORT)
#define RASM_SSE_SUPPORT_ENTRY_HOOK()
#define RASM_SSE_SUPPORT_EXIT_HOOK()

#elif defined(RASM_SSE_SUPPORT)
#define RASM_SSE_SUPPORT_ENTRY_HOOK() rasm_sse_support_copy_from_shelter_regs()
#define RASM_SSE_SUPPORT_EXIT_HOOK() rasm_sse_support_copy_to_shelter_regs()

#elif defined(RASM_SSE2_SUPPORT)
#define RASM_SSE_SUPPORT_ENTRY_HOOK() rasm_sse2_support_copy_from_shelter_regs()
#define RASM_SSE_SUPPORT_EXIT_HOOK() rasm_sse2_support_copy_to_shelter_regs()
#endif


#endif
