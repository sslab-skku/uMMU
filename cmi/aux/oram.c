#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORAM_BLOCK_SIZE 64 // byte
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

oram_blk_arr *oram_alloc_blk_arr(int len) {
  oram_blk_arr *blk_arr = malloc(sizeof(oram_blk_arr));
  blk_arr->data = calloc(len, sizeof(oram_blk_data));
  blk_arr->id = calloc(len, sizeof(int));
  blk_arr->len = len;
  return blk_arr;
}

void oram_blk_arr_copy(oram_blk_arr *dst, int didx, oram_blk_arr *src,
                       int sidx) {
  memcpy(&dst->data[didx], &src->data[sidx], sizeof(oram_blk_data));
  memcpy(&dst->id[didx], &src->id[sidx], sizeof(int));
}

int P(int L, int x, int l) { return (x >> (L - l)) + (1 << l) - 1; }

oram_ctx *oram_alloc_ctx(int Z, int L) {
  oram_ctx *ctx = malloc(sizeof(oram_ctx));
  int N = (1 << (L + 1)) - 1;
  ctx->L = L;
  ctx->Z = Z;
  ctx->N = N;

  // Allocate observable bucket block storage
  ctx->bucket = oram_alloc_blk_arr(Z * N);
  for (int i = 0; i < ctx->bucket->len; i++) {
    ctx->bucket->id[i] = ORAM_EMPTY_BID;
  }

  // Allocate Stash
  // FIXME: Appromximated stash size decision, not guranteed to work correctly.
  ctx->stash = oram_alloc_blk_arr(200);
  for (int i = 0; i < ctx->stash->len; i++) {
    ctx->stash->id[i] = ORAM_EMPTY_BID;
  }

  // Position Map
  ctx->position = calloc(N, sizeof(int));
  for (int i = 0; i < N; i++) {
    ctx->position[i] = rand() % (1 << L);
  }
  return ctx;
}

int oram_blk_arr_find(oram_blk_arr *blk_arr, int *it, int bid) {
  for (int cnt = 0; cnt < blk_arr->len; cnt++) {
    if (blk_arr->id[*it] == bid)
      return 0;
    *it = ((*it) + 1) % blk_arr->len;
  }
  return -1;
}

void oram_stash_read_bucket_block(oram_ctx *ctx, int bid, int *sit) {}

void oram_access(oram_ctx *ctx, int is_write, int bid, oram_blk_data *data) {
  const int L = ctx->L, N = ctx->N, Z = ctx->Z;
  int *const position = ctx->position;
  oram_blk_arr *const S = ctx->stash;
  oram_blk_arr *const bkt = ctx->bucket;
  oram_blk_data blk_buf;
  int sit = 0;        // stash iterator
  int s_bid_idx = -1; // index of bid in S
  int x = position[bid];
  position[bid] = rand() % (1 << L);

  // for l in {0,...,L}
  for (int l = 0; l <= L; l++) {
    int bkt_blk0_idx = Z * P(L, x, l);
    // for every block in bucket P(L,x,l)
    for (int bidx = bkt_blk0_idx; bidx < bkt_blk0_idx + Z; bidx++) {
      if (oram_blk_arr_find(ctx->stash, &sit, bkt->id[bidx]) < 0) {
        if (oram_blk_arr_find(ctx->stash, &sit, ORAM_EMPTY_BID) < 0) {
          fprintf(stderr, "%s: Stash is full len=%d \n", __func__,
                  ctx->stash->len);
          exit(-1);
        };
      }
      oram_blk_arr_copy(ctx->stash, sit, ctx->bucket, bidx);
    }
  }

  // if bid not exists in stash create one
  if (oram_blk_arr_find(S, &sit, bid) < 0) {
    if (oram_blk_arr_find(S, &sit, ORAM_EMPTY_BID) < 0) {
      fprintf(stderr, "Block init: Stash is full\n");
      exit(-1);
    }
    S->id[sit] = bid;
  }
  // now sit points to bid

  // write or read from stash
  if (is_write) {
    memcpy(&S->data[sit], data, sizeof(oram_blk_data));
  } else {
    memcpy(data, &S->data[sit], sizeof(oram_blk_data));
  }

  // evection
  for (int l = L; l >= 0; l--) {
    int select[100]; // FIXME
    int select_top = 0;
    // S' = P(x,l) = P(position[a'],l)
    for (sit = 0; sit < S->len; sit++) {
      int a2 = S->id[sit];
      if (P(L, x, l) == P(L, position[a2], l)) {
        select[select_top++] = sit;
      }
      if (select_top > Z) {
        break;
      }
    }

    // write bucket
    int bkt_blk0_idx = Z * P(L, x, l);
    for (int i = 0; i < Z; i++) {
      int bidx = bkt_blk0_idx + i;
      if (i < select_top) {
        oram_blk_arr_copy(bkt, bidx, S, select[i]);
        S->id[select[i]] = ORAM_EMPTY_BID;
      } else { // fill dummy blocks
        memset(&bkt->data[bidx], 0, sizeof(oram_blk_data));
        bkt->id[bidx] = ORAM_EMPTY_BID;
      }
    }
  }
}

// oram_read/write: oram_access wrapper functions
void oram_read(oram_ctx *ctx, void *dst, void *src, int size) {
  oram_blk_data buf;
  int src_bid = ((uint64_t)src) / sizeof(oram_blk_data);
  int src_off = ((uint64_t)src) % sizeof(oram_blk_data);
  oram_access(ctx, 0, src_bid, &buf);
  memcpy(dst, ((char *)&buf) + src_off, size);
}

void oram_write(oram_ctx *ctx, void *dst, void *src, int size) {
  oram_blk_data buf;
  int dst_bid = ((uint64_t)dst) / sizeof(oram_blk_data);
  int dst_off = ((uint64_t)dst) % sizeof(oram_blk_data);
  oram_access(ctx, 0, dst_bid, &buf);
  memcpy(((char *)&buf) + dst_off, src, size);
  oram_access(ctx, 1, dst_bid, &buf);
}

int main() {
  oram_ctx *ctx = oram_alloc_ctx(4, 10);
  oram_blk_data buf;
  int i = 0;

  printf("TEST1: single read write test\n");
  int val = 3, val2 = -1;
  oram_write(ctx, (void *)0, &val, sizeof(val));
  oram_read(ctx, &val2, (void *)0, sizeof(val));
  if (val2 != val) {
    fprintf(stderr, "FAIL val2=%d\n", val2);
    exit(-1);
  }

  printf("PASS1\n");
  oram_write(ctx, (void *)0, &val, sizeof(val));
  oram_read(ctx, &val2, (void *)0, sizeof(val));
  if (val2 != val) {
    fprintf(stderr, "FAIL val2=%d\n", val2);
    exit(-1);
  }
  printf("PASS2\n");

  int rep = 10000;
  int *oarr = (void *)0;
  for (int i = 0; i < rep; i++) {
    int val = i;
    oram_write(ctx, &oarr[i], &val, sizeof(val));
  }
  for (int i = 0; i < rep; i++) {
    int val;
    oram_read(ctx, &val, &oarr[i], sizeof(val));
    if (val != i) {
      fprintf(stderr, "FAIL\n");
      exit(-1);
    }
    val = (val * val) % 1000;
    oram_write(ctx, &oarr[i], &val, sizeof(val));
  }
  for (int i = 0; i < rep; i++) {
    int val;
    oram_read(ctx, &val, &oarr[i], sizeof(val));
    if (val != (i * i) % 1000) {
      fprintf(stderr, "FAIL\n");
      exit(-1);
    }
  }

  printf("PASS3\n");

  return 0;
}
