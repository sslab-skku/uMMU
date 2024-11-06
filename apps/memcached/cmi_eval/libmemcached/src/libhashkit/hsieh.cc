/*
    +--------------------------------------------------------------------+
    | libmemcached-awesome - C/C++ Client Library for memcached          |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted under the terms of the BSD license.    |
    | You should have received a copy of the license in a bundled file   |
    | named LICENSE; in case you did not receive a copy you can review   |
    | the terms online at: https://opensource.org/licenses/BSD-3-Clause  |
    +--------------------------------------------------------------------+
    | Copyright (c) 2006-2014 Brian Aker   https://datadifferential.com/ |
    | Copyright (c) 2020-2021 Michael Wallner        https://awesome.co/ |
    +--------------------------------------------------------------------+
*/

#include "libhashkit/common.h"

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__))
#  define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined(get16bits)
#  define get16bits(d) \
    ((((uint32_t)(((const uint8_t *) (d))[1])) << 8) + (uint32_t)(((const uint8_t *) (d))[0]))
#endif

#ifdef HAVE_HSIEH_HASH
uint32_t hashkit_hsieh(const char *key, size_t key_length, void *) {
  uint32_t hash = 0, tmp;
  int rem;

  if (key_length <= 0 || key == NULL)
    return 0;

  rem = key_length & 3;
  key_length >>= 2;

  /* Main loop */
  for (; key_length > 0; key_length--) {
    hash += get16bits(key);
    tmp = (get16bits(key + 2) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    key += 2 * sizeof(uint16_t);
    hash += hash >> 11;
  }

  /* Handle end cases */
  switch (rem) {
  case 3:
    hash += get16bits(key);
    hash ^= hash << 16;
    hash ^= (uint32_t) key[sizeof(uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2:
    hash += get16bits(key);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1:
    hash += (unsigned char) (*key);
    hash ^= hash << 10;
    hash += hash >> 1;
  default:
    break;
  }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}
#else
uint32_t hashkit_hsieh(const char *, size_t, void *) {
  return 0;
}
#endif
