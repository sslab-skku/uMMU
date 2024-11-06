#include "rdrand.h"
#include "vmi_page.h"
#include <cmi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test_utils.h>

#define HASH_MASK (0xff)

#ifdef TEST_CMI_USE_DUMMY
#define X__swap(x, y, z)                                                       \
  do {                                                                         \
  } while (0)
#define X__load(x) (0)
#define X__store(x, y)                                                         \
  do {                                                                         \
  } while (0)
#define X__load64 X__load
#define X__load32 X__load
#define X__load16 X__load
#define X__load8 X__load
#define X__store64 X__store
#define X__store32 X__store
#define X__store16 X__store
#define X__store8 X__store

#else
#ifdef TEST_CMI_X_USE_BASE
#define X__swap __cmi_swap
#define X__load64 __cmi_load64
#define X__load32 __cmi_load32
#define X__load16 __cmi_load16
#define X__load8 __cmi_load8
#define X__store64 __cmi_store64
#define X__store32 __cmi_store32
#define X__store16 __cmi_store16
#define X__store8 __cmi_store8
#else
#define X__swap __cmi_swap
#define X__load64 __cmi_load64
#define X__load32 __cmi_load32
#define X__load16 __cmi_load16
#define X__load8 __cmi_load8
#define X__store64 __cmi_store64
#define X__store32 __cmi_store32
#define X__store16 __cmi_store16
#define X__store8 __cmi_store8
#endif
#endif

// clang-format off
#define X__loadi64(_IPTR)   ((int64_t)X__load64((uint64_t *)_IPTR))
#define X__loadi32(_IPTR)   ((int32_t)X__load32((uint32_t *)_IPTR))
#define X__loadi16(_IPTR)   ((int16_t)X__load16((uint16_t *)_IPTR))
#define X__loadi8(_IPTR)    ((int8_t)X__load8((uint8_t *)_IPTR))

#define X__storei64(_IPTR, _IVAL)   X__store64(((uint64_t *)_IPTR), (int64_t)_IVAL)
#define X__storei32(_IPTR, _IVAL)   X__store32(((uint32_t *)_IPTR), (int32_t)_IVAL)
#define X__storei16(_IPTR, _IVAL)   X__store16(((uint16_t *)_IPTR), (int16_t)_IVAL)
#define X__storei8(_IPTR, _IVAL)    X__store8(((uint8_t *)_IPTR), (int8_t)_IVAL)


#define HEADER_FORMAT(A, B, C, D)    printf("HEADER, %25s, %13s, %5s, %10s\n",  A, B, C, D);
#define RESULT_FORMAT(A, B, C, D)       printf("RESULT, %25s, %13lu, %5x, %10d\n", A, B, C, D);
// clang-format on

void random_swap_test_u64(uint64_t arr[], int len, int count) {
  uint64_t a = 0, b = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    j = FAST_RAND(len);
    k = FAST_RAND(len);
    // SWAP
    X__swap(arr[j], arr[k], 64);
  }
}

uint64_t random_load_test_u64(uint64_t arr[], int len, int count) {
  uint64_t ret = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    j = FAST_RAND(len);
    k = i % len;
    ret ^= X__load64(&arr[j]);
  }
  return ret;
}

uint64_t linear_load_test_u64(uint64_t arr[], int len, int count) {
  uint64_t ret = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    k = FAST_RAND(len);
    j = i % len;
    ret ^= X__load64(&arr[j]);
  }
  return ret;
}

uint64_t random_store_test_u64(uint64_t arr[], int len, int count) {
  uint64_t ret = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    j = FAST_RAND(len);
    k = i % len;
    X__store64(&arr[j], k);
  }
  return ret;
}

uint64_t linear_store_test_u64(uint64_t arr[], int len, int count) {
  uint64_t ret = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    k = FAST_RAND(len);
    j = i % len;
    // j = FAST_RAND(len);
    X__store64(&arr[j], k);
  }
  return ret;
}

uint64_t random_load_test_u64x8(uint64_t arr[], int len, int count) {
  uint64_t ret = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    j = (FAST_RAND((len - 8) / 8)) * 8;
    ret ^= X__load64(&arr[j]);
    ret ^= X__load64(&arr[j + 1]);
    ret ^= X__load64(&arr[j + 2]);
    ret ^= X__load64(&arr[j + 3]);
    ret ^= X__load64(&arr[j + 4]);
    ret ^= X__load64(&arr[j + 5]);
    ret ^= X__load64(&arr[j + 6]);
    ret ^= X__load64(&arr[j + 7]);
  }
  return ret;
}

void random_swap_test_u8(uint8_t arr[], int len, int count) {
  uint8_t a = 0, b = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    j = FAST_RAND(len);
    k = FAST_RAND(len);
    // SWAP
    a = X__load8(&arr[j]);
    b = X__load8(&arr[k]);
    X__store8(&arr[j], b);
    X__store8(&arr[k], a);
  }
}

void linear_swap_test_u8(uint8_t arr[], int len, int count) {
  uint8_t a = 0, b = 0;
  int i = 0, j = 0, k = 0;
  while (i++ < count) {
    j = i % len;
    k = (i + 1) % len;
    // SWAP
    X__swap(arr[j], arr[k], 8);
  }
}

/* Quick Sort */
int qsort_u32_partition(uint32_t arr[], int low, int high) {
  // choose the pivot
  uint32_t pivot = X__load32(&arr[high]);
  // Index of smaller element and Indicate
  // the right position of pivot found so far
  int i = (low - 1);

  for (int j = low; j <= high; j++) {
    // If current element is smaller than the pivot
    if (X__load32(&arr[j]) < pivot) {
      // Increment index of smaller element
      i++;
      X__swap(arr[i], arr[j], 32);
    }
  }
  X__swap(arr[i + 1], arr[high], 32);
  return (i + 1);
}

void qsort_u32(uint32_t arr[], int low, int high) {
  // when low is less than high
  if (low < high) {
    // pi is the partition return index of pivot

    int pi = qsort_u32_partition(arr, low, high);

    // Recursion Call
    // smaller element than pivot goes left and
    // higher element goes right
    qsort_u32(arr, low, pi - 1);
    qsort_u32(arr, pi + 1, high);
  }
}

/* Quick Sort */
int qsort_u64_partition(uint64_t arr[], int low, int high) {
  // choose the pivot
  uint64_t pivot = X__load64(&arr[high]);
  // Index of smaller element and Indicate
  // the right position of pivot found so far
  int i = (low - 1);

  for (int j = low; j <= high; j++) {
    // If current element is smaller than the pivot
    if (X__load64(&arr[j]) < pivot) {
      // Increment index of smaller element
      i++;
      X__swap(arr[i], arr[j], 64);
    }
  }
  X__swap(arr[i + 1], arr[high], 64);
  return (i + 1);
}

void qsort_u64(uint64_t arr[], int low, int high) {
  // when low is less than high
  if (low < high) {
    // pi is the partition return index of pivot

    int pi = qsort_u64_partition(arr, low, high);

    // Recursion Call
    // smaller element than pivot goes left and
    // higher element goes right
    qsort_u64(arr, low, pi - 1);
    qsort_u64(arr, pi + 1, high);
  }
}

// Find Max
static uint32_t find_max_u32(uint32_t arr[], uint32_t n) {
  uint32_t mx = X__load32(&arr[0]);
  for (uint32_t i = 1; i < n; i++) {
    uint32_t t = X__load32(&arr[i]);
    if (t > mx)
      mx = t;
  }
  return mx;
}

// Radix Sort output[n]
static void csort_u32(uint32_t output[], uint32_t count[], uint32_t arr[],
                      uint32_t n, uint32_t exp) {
  // uint32_t output[n]; // output array
  //  uint32_t count[10] = {0};
  int i = 0;
  for (i = 0; i < 10; i++) {
    X__store32(&count[i], 0);
  }

  // Store count of occurrences in count[]
  for (i = 0; i < n; i++) {
    uint32_t t = X__load32(&arr[i]);
    uint32_t c = X__load32(&count[(t / exp) % 10]);
    c++;
    X__store32(&count[(t / exp) % 10], c);
    // count[(t / exp) % 10]++;
  }

  // Change count[i] so that count[i] now contains actual
  //  position of this digit in output[]
  for (i = 1; i < 10; i++) {
    // count[i] += count[i - 1];
    uint32_t c1 = X__load32(&count[i]);
    uint32_t c2 = X__load32(&count[i - 1]);
    X__store32(&count[i], c1 + c2);
  }

  // Build the output array
  for (i = n - 1; i >= 0; i--) {
    uint32_t t = X__load32(&arr[i]);
    uint32_t c1 = X__load32(&count[i]);
    uint32_t c = X__load32(&count[(t / exp) % 10]);
    X__store32(&output[c - 1], t);

    c--;
    X__store32(&count[(t / exp) % 10], c);
    // count[(t / exp) % 10]--;
  }

  // Copy the output array to arr[], so that arr[] now
  // contains sorted numbers according to current digit
  for (i = 0; i < n; i++) {
    X__store32(&arr[i], X__load32(&output[i]));
    //__cmi_store32(&arr[i], output[i]);
  }
}

// The main function to that sorts arr[] of size n using
// Radix Sort
static void rsort(uint32_t output[], uint32_t count[], uint32_t arr[],
                  uint32_t n) {
  // Find the maximum number to know number of digits
  uint32_t m = find_max_u32(arr, n);

  // Do counting sort for every digit. Note that instead
  // of passing digit number, exp is passed. exp is 10^i
  // where i is current digit number
  for (uint64_t exp = 1; ((uint64_t)m > exp) && ((1 << 31) > exp); exp *= 10)
    csort_u32(output, count, arr, n, exp);
}

uint64_t binsearch_u64(uint64_t arr[], int left, int right, uint64_t target) {
  while (left <= right) {
    int mid = left + (right - left) / 2;

    uint64_t midVal = X__load64(&arr[mid]);
    if (midVal == target) {
      return mid;
    } else if (midVal < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1; // Target not found
}

#define DIJK_MAX_INPUT_SIZE 1024 // 16KB

int dijkstra_write_only(int X, int s, int *e, int n, int *vis, int *dis) {
  /* int vis[DIJK_MAX_INPUT_SIZE] = {0}; */
  /* int dis[DIJK_MAX_INPUT_SIZE] = {0}; */
  // int s = 0;
  int t = n - 1;
  int bestj = -1;

  uint32_t ta, tb;
  X__storei32(&vis[s], 1); // vis[s] = 1;
  for (int i = 0; i < n; ++i) {
    // X__storei32(&dis[i], (X__loadi32((e + (n * s) + i)))); // dis[i] =
    // e[s][i];
    X__storei32(&dis[i], *(e + (n * s) + i)); // dis[i] = e[s][i];
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (!X__loadi32(&vis[j]) &&
          (bestj < 0 || X__loadi32(&dis[j]) < X__loadi32(&dis[bestj]))) {
        bestj = j + X;               // fix implicit flow here
        X__storei32(&vis[bestj], 1); // vis[bestj] = 1;
      }
      if (bestj > 0) {
        X__storei32(&vis[bestj], 1); // vis[bestj] = 1;
      }
    }
    for (int j = 0; j < n; ++j) {
      int disj = X__loadi32(&dis[bestj]);
      int ejj = *(e + (n * bestj) + j); // e[bestj][j]
      if (!X__loadi32(&vis[j]) && (disj + ejj < X__loadi32(&dis[j])))
        X__storei32(&dis[j], (disj + ejj));
    }
  }
  return X__loadi32(&dis[t]);
}

int dijkstra(int X, int s, int *e, int n, int *vis, int *dis) {
  /* int vis[DIJK_MAX_INPUT_SIZE] = {0}; */
  /* int dis[DIJK_MAX_INPUT_SIZE] = {0}; */
  // int s = 0;
  int t = n - 1;
  int bestj = -1;

  uint32_t ta, tb;
  X__storei32(&vis[s], 1); // vis[s] = 1;
  for (int i = 0; i < n; ++i) {
    X__storei32(&dis[i], (X__loadi32((e + (n * s) + i)))); // dis[i] = e[s][i];
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      if (!X__loadi32(&vis[j]) &&
          (bestj < 0 || X__loadi32(&dis[j]) < X__loadi32(&dis[bestj]))) {
        bestj = j + X;               // fix implicit flow here
        X__storei32(&vis[bestj], 1); // vis[bestj] = 1;
      }
    for (int j = 0; j < n; ++j) {
      int disj = X__loadi32(&dis[bestj]);
      int ejj = X__loadi32((e + (n * bestj) + j)); // e[bestj][j]
      if (!X__loadi32(&vis[j]) && (disj + ejj < X__loadi32(&dis[j])))
        X__storei32(&dis[j], (disj + ejj));
    }
  }
  return X__loadi32(&dis[t]);
}

void matmul_i32(int n, int *a, int *b, int *c) {
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      X__storei32((c + (n * i) + j), 0); // c[i][j] = 0;
      for (int k = 0; k < n; ++k) {
        // c[i][j] += a[i][k] * b[k][j];
        X__storei32((c + (n * i) + j), (X__loadi32((c + (n * i) + j))) +
                                           (X__loadi32((a + (n * i) + k)) *
                                            X__loadi32((b + (n * k) + j))));
      }
    }
}

void hist_i32(int n, int a[], int c[]) {
  int i;
  int t, v;
  for (i = 0; i < n; i++)
    X__storei32(&c[i], 0);
  i = 0;
  for (i = 0; i < n; i++) {
    v = X__loadi32(&a[i]);
    if (v > 0)
      t = v % n;
    else
      t = (0 - v) % n;
    X__storei32(&c[t], (X__loadi32(&c[t]) + 1)); // c[t] = c[t] + 1;
  }
}

int main(int argc, char *argv[]) {
  int len, count, algo_cnt, seed;
  uint8_t *cmi_arr;
  uint8_t *cmi_arr8;
  uint32_t *cmi_arr32;
  uint64_t *cmi_arr64;
  int arr8_len, arr32_len, arr64_len;
  uint8_t *buf_arr;
  uint32_t res, h1, h2;
  uint64_t t1, t2, tsum;
  char *target = NULL;
  len = (argc > 1) ? atoi(argv[1]) : 1700;
  count = (argc > 2) ? atoi(argv[2]) : 1000000;
  algo_cnt = (argc > 3) ? atoi(argv[3]) : 100;
  target = (argc > 4) ? argv[4] : "all";
  seed = (argc > 5) ? atoi(argv[5]) : 1337;
  // printf("len=%-10d count=%-10d seed=%-10d\n", len, count, seed);

  cmi_arr = __cmi_malloc(len + 64);
  buf_arr = malloc(len);

  cmi_arr8 = (uint8_t *)cmi_arr;
  cmi_arr32 = (uint32_t *)cmi_arr;
  cmi_arr64 = (uint64_t *)cmi_arr;
  arr8_len = len;
  arr32_len = (len / 4) - 1;
  arr64_len = (len / 8) - 1;

  init_random_arr(buf_arr, len, seed);
  h1 = hash32((char *)buf_arr, len);
  copy_to_cmi(cmi_arr, buf_arr, len);
  h2 = hash32_cmi_arr(cmi_arr, len);
  if (h1 != h2) {
    fprintf(stderr, "[ERR] init_arr, h1!=h2\n");
    fflush(stdout);
    // return -1;
  }

  // Start Tests
  if (!strcmp(target, "all")) {
    HEADER_FORMAT("TEST", "TICK", "HASH", "REP");
  }
  setbuf(stdout, NULL);
  // warm up
  if (algo_cnt > 10) {
    random_load_test_u64((uint64_t *)cmi_arr, 10, 1000000);
  }
  fflush(stdout);

  if (!strcmp(target, "micro")) {
    t1 = get_tick();
    for (int i; i < count; i++) {
      h1 ^= X__load64(cmi_arr64);
    }
    t2 = get_tick();
    RESULT_FORMAT("single_load_test_u64", t2 - t1, (h1 & HASH_MASK), count);
    t1 = get_tick();
    for (int i; i < count; i++) {
      X__store64(cmi_arr64, i);
    }
    t2 = get_tick();
    RESULT_FORMAT("single_store-test_u64", t2 - t1, (h1 & HASH_MASK), count);
    fflush(stdout);
  }
  if (!strcmp(target, "rand")) {
    // init_random_arr(buf_arr, len, seed + i);
    int *rand_idx = calloc(count, sizeof(int));
    for (int i = 0; i < count; i++) {
      rand_idx[i] = (((int)fast_rand()) % arr64_len)-1;
    }
    t1 = get_tick();
    for (int i = 0; i < count; i++) {
      h1^=X__load64(cmi_arr64+ rand_idx[i]);
    }
    t2 = get_tick();
    RESULT_FORMAT("rand", t2 - t1, (h1 & HASH_MASK), count);

  }

  // Random Load u64 Test
  if (!strcmp(target, "mem")) {
    t1 = get_tick();
    for (int i = 0; i < count; i++) {
      h1 ^= cmi_arr[0];
    }
    h1 = random_load_test_u64((uint64_t *)cmi_arr, len / 8 - 1, count);
    t2 = get_tick();
    RESULT_FORMAT("random_load_test_u64_hash", t2 - t1, (h1 & HASH_MASK),
                  count);

    // Random Store u64 Test
    t1 = get_tick();
    h1 = random_store_test_u64((uint64_t *)cmi_arr, len / 8 - 1, count);
    t2 = get_tick();
    h1 = hash32_cmi_arr(cmi_arr, len);
    RESULT_FORMAT("random_store_test_u64", t2 - t1, (h1 & HASH_MASK), count);
    fflush(stdout);

    // Linear Load u64 Test
    t1 = get_tick();
    h1 = linear_load_test_u64((uint64_t *)cmi_arr, len / 8 - 1, count);
    t2 = get_tick();
    RESULT_FORMAT("linear_load_test_u64", t2 - t1, (h1 & HASH_MASK), count);
    fflush(stdout);

    // Linear Store u64 Test
    t1 = get_tick();
    h1 = linear_store_test_u64((uint64_t *)cmi_arr, len / 8 - 1, count);
    t2 = get_tick();
    h1 = hash32_cmi_arr(cmi_arr, len);

    RESULT_FORMAT("linear_store_test_u64", t2 - t1, (h1 & HASH_MASK), count);
    fflush(stdout);
  }

#ifdef TEST_LOAD64X8
  // Random Load u64x8 Test
  t1 = get_tick();
  h1 = random_load_test_u64x8((uint64_t *)cmi_arr, len / 8 - 1, count);
  t2 = get_tick();

  RESULT_FORMAT("random_load_test_u64x8", t2 - t1, (h1 & HASH_MASK), count);
  fflush(stdout);

#endif

#ifdef TEST_SWAP
  // Swap test
  t1 = get_tick();
  random_swap_test_u64((uint64_t *)cmi_arr, len / 8 - 1, count);
  t2 = get_tick();
  h1 = hash32_cmi_arr(cmi_arr, len);
  RESULT_FORMAT("random_swap_test_u64", t2 - t1, (h1 & HASH_MASK), count);
  fflush(stdout);

  // random_swap_test_u8
  t1 = get_tick();
  random_swap_test_u8((uint8_t *)cmi_arr, len, count);
  t2 = get_tick();
  h1 = hash32_cmi_arr(cmi_arr, len);
  RESULT_FORMAT("random_swap_test_u8", t2 - t1, (h1 & HASH_MASK), count);
  fflush(stdout);

  // Linear swap test
  t1 = get_tick();
  linear_swap_test_u8((uint8_t *)cmi_arr, len, count);
  t2 = get_tick();
  h1 = hash32_cmi_arr(cmi_arr, len);
  RESULT_FORMAT("linear_swap_test_u8", t2 - t1, (h1 & HASH_MASK), count);
#endif

  // Binary Search 64
  if (!strcmp(target, "bin") || !strcmp(target, "all")) {
    tsum = 0;
    // Init random array
    init_random_arr(buf_arr, len, seed);
    copy_to_cmi(cmi_arr, buf_arr, len);
    // Sort cmi array
    qsort_u64(cmi_arr64, 0, arr64_len);

    vmi_measure_start();
    for (int i = 0; i < count; i++) {
      uint64_t target = ((uint64_t *)buf_arr)[FAST_RAND(arr64_len)];
      t1 = get_tick();
      h1 ^= binsearch_u64(cmi_arr64, 0, arr64_len, target);
      t2 = get_tick();
      tsum += t2 - t1;
    }
    vmi_measure_stop();
    vmi_print_stats();
    RESULT_FORMAT("binsearch_u64", tsum, (h1 & HASH_MASK), count);
  }

  // DIJKSTRA
  vmi_init_stats();
  h1 = 0;
  if (!strcmp(target, "dijk") || !strcmp(target, "all")) {
    tsum = 0;
    for (int i = 0; i < algo_cnt; i++) {
      // Init random array
      init_random_arr(buf_arr, len, seed + i);
      copy_to_cmi(cmi_arr, buf_arr, len);
      int DN = 0;

      while ((DN * DN + DN + DN + 1) < arr32_len) {
        DN++;
      }

      vmi_measure_start();
      t1 = get_tick();
      for (int i = 0; i < DN * 2; i++) {
        X__store32(&cmi_arr32[i], 0);
      }
      h1 ^= dijkstra(0, FAST_RAND(DN), (int *)cmi_arr + DN * 2, DN,
                     (int *)cmi_arr, (int *)cmi_arr + DN);
      t2 = get_tick();
      tsum += t2 - t1;
      vmi_measure_stop();
    }
    vmi_print_stats();
    RESULT_FORMAT("dijkstra", tsum, (h1 & HASH_MASK), algo_cnt);
  }

  // Dijkstra write only
  vmi_init_stats();
  h1 = 0;
  if (!strcmp(target, "dijk_wo") || !strcmp(target, "all")) {
    int DN = 0;
    while (2 * (DN + 1) < arr32_len) {
      DN++;
    }
    int *Earr = calloc(DN * DN, sizeof(int));
    tsum = 0;
    for (int i = 0; i < algo_cnt; i++) {
      // Init random array
      init_random_arr((uint8_t *)Earr, DN * DN * sizeof(int), seed + i);
      // copy_to_cmi(cmi_arr, buf_arr, len);
      int *vis = (int *)cmi_arr32;
      int *dis = (int *)(cmi_arr32 + DN);

      vmi_measure_start();
      t1 = get_tick();
      for (int i = 0; i < DN * 2; i++) {
        X__store32(&cmi_arr32[i], 0);
      }
      h1 ^= dijkstra_write_only(0, FAST_RAND(DN), Earr, DN, vis, dis);
      t2 = get_tick();
      tsum += t2 - t1;
      vmi_measure_stop();
    }
    vmi_print_stats();
    RESULT_FORMAT("dijkstra_wo", tsum, (h1 & HASH_MASK), algo_cnt);
  }

  // HISTOGRAM
  vmi_init_stats();
  h1 = 0;
  if (!strcmp(target, "hist") || !strcmp(target, "all")) {
    tsum = 0;
    for (int i = 0; i < algo_cnt; i++) {
      // Init random array
      init_random_arr(buf_arr, len, seed + i);
      copy_to_cmi(cmi_arr, buf_arr, len);
      vmi_measure_start();
      t1 = get_tick();
      hist_i32(arr32_len / 2, ((int *)cmi_arr),
               ((int *)cmi_arr) + arr32_len / 2);
      t2 = get_tick();
      vmi_measure_stop();
      h1 ^= hash32_cmi_arr(cmi_arr, len);
      tsum += t2 - t1;
    }
    vmi_print_stats();
    RESULT_FORMAT("hist", tsum, (h1 & HASH_MASK), algo_cnt);
  }
  // MatMul
  vmi_init_stats();
  h1 = 0;
  if (!strcmp(target, "matmul") || !strcmp(target, "all")) {
    // Init random array
    init_random_arr(buf_arr, len, seed);
    copy_to_cmi(cmi_arr, buf_arr, len);
    tsum = 0;

    for (int i = 0; i < algo_cnt; i++) {
      int N = 0;
      while ((N * N * 3) < arr32_len) {
        N++;
      }
      vmi_measure_start();
      t1 = get_tick();
      matmul_i32(N, ((int *)cmi_arr) + 0, ((int *)cmi_arr) + N * N,
                 ((int *)cmi_arr) + N * N * 2);
      t2 = get_tick();
      tsum += t2 - t1;
    }
    vmi_measure_stop();
    vmi_print_stats();
    h1 = hash32_cmi_arr(cmi_arr, len);
    RESULT_FORMAT("matmul", tsum, (h1 & HASH_MASK), algo_cnt);
  }

  // Quick Sort Test 32
  vmi_init_stats();
  if (!strcmp(target, "qsort32") || !strcmp(target, "all")) {
    tsum = 0;
    for (int i = 0; i < algo_cnt; i++) {
      // Init random array
      init_random_arr(buf_arr, len, seed + i);
      copy_to_cmi(cmi_arr, buf_arr, len);
      vmi_measure_start();
      t1 = get_tick();
      qsort_u32((uint32_t *)cmi_arr, 0, len / 4 - 2);
      t2 = get_tick();
      vmi_measure_stop();
      tsum += t2 - t1;
      h1 ^= hash32_cmi_arr(cmi_arr, len);
    }
    vmi_print_stats();

    RESULT_FORMAT("qsort_u32", tsum, (h1 & HASH_MASK), algo_cnt);
  }
  // Quick Sort Test 64
  vmi_init_stats();
  if (!strcmp(target, "qsort64") || !strcmp(target, "all")) {
    tsum = 0;
    algo_cnt = algo_cnt;
    for (int i = 0; i < algo_cnt; i++) {
      // Init random array
      init_random_arr(buf_arr, len, seed + i);
      copy_to_cmi(cmi_arr, buf_arr, len);
      vmi_measure_start();
      t1 = get_tick();
      qsort_u64((uint64_t *)cmi_arr, 0, (len / 8) - 1);
      t2 = get_tick();
      vmi_measure_stop();
      tsum += t2 - t1;
    }
    vmi_print_stats();
    h1 = hash32_cmi_arr(cmi_arr, len);
    RESULT_FORMAT("qsort_u64", tsum, (h1 & HASH_MASK), algo_cnt);
  }

  // Radix Sort Test
  vmi_init_stats();
  if (!strcmp(target, "rsort") || !strcmp(target, "all")) {
    tsum = 0;
    algo_cnt = algo_cnt;
    for (int i = 0; i < algo_cnt; i++) {
      // Init random array
      init_random_arr(buf_arr, len, seed + i);
      copy_to_cmi(cmi_arr, buf_arr, len);
      uint32_t N = 0;
      while ((N * 2 + 10 + 3) < arr32_len) {
        N++;
      }
      N--;
      vmi_measure_start();
      t1 = get_tick();
      rsort(cmi_arr32, cmi_arr32 + N, cmi_arr32 + N + 2, N);
      t2 = get_tick();
      tsum += t2 - t1;
      vmi_measure_stop();
    }
    vmi_print_stats();
    h1 = hash32_cmi_arr(cmi_arr, len);
    RESULT_FORMAT("rsort", tsum, (h1 & HASH_MASK), algo_cnt);
  }

  return 0;
}
