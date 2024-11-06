#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>

extern void encrypt_memory(void *);
extern void decrypt_memory(void *);
extern void nop_func(void);

extern uint64_t aes_load(void *);
extern void aes_store(void *, uint64_t);

extern uint64_t aes_xmm3_load(void *);
extern void aes_xmm3_store(void *, uint64_t);

extern uint64_t fake_aes_load(void *);
extern void fake_aes_store(void *, uint64_t);
extern void aes_populate_key();

#endif
