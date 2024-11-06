#ifndef ORAM_H
#define ORAM_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ORAM_BLOCK_SIZE
#define ORAM_BLOCK_SIZE 64 // byte
#endif

#ifndef ORAM_STASH_SIZE
#define ORAM_STASH_SIZE 200
#endif


#define ORAM_DUMMY_BID (-2)
#define ORAM_EMPTY_BID (-1)


typedef struct oram_blk_data {
  uint8_t raw[ORAM_BLOCK_SIZE];
} oram_blk_data;

typedef struct oram_blk_arr {
  oram_blk_data *data;
  int *id;
  int len;
} oram_blk_arr;

typedef struct oram_ctx {
  int L;
  int Z;
  int N;
  oram_blk_arr *bucket;
  oram_blk_arr *stash;
  int *position;
} oram_ctx;

extern void oram_read(oram_ctx *ctx, void *dst, void *src, int size);
extern void oram_write(oram_ctx *ctx, void *dst, void *src, int size);
extern oram_ctx *oram_alloc_ctx(int Z, int L);

#endif
