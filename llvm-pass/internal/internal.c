#include "cmi.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <unistd.h>

typedef _Bool uint1_t;

typedef unsigned long size_t;

/********* TLIBC ********/
// clang-format off
void tlibc_internal_explicit_bzero(void* p, size_t n) __attribute__((used));
void tlibc_internal_memset(void *s, char c, size_t n, uint1_t isVolatile) __attribute__((used));
int tlibc_internal_memcmp(const void *s1, const void *s2, size_t n) __attribute__((used));
void tlibc_internal_memcpy(void *dest, const void *src, size_t n, uint1_t isVolatile) __attribute__((used));
void tlibc_internal_memmove(void *dest, const void *src, size_t n, uint1_t isVolatile) __attribute__((used));
size_t tlibc_internal_strlen(const char *s) __attribute__((used));
char* tlibc_internal_strcat(char* destination, const char* source) __attribute__((used));
char* tlibc_internal_strcpy(char* destination, const char* source) __attribute__((used));
int tlibc_internal_strcmp(const char *X, const char *Y) __attribute__((used));
char* tlibc_internal_strstr(const char *str, const char *substr) __attribute__((used));

ssize_t tlibc_internal_read(int fd, void* buf, size_t count) __attribute__((used));
ssize_t tlibc_recv(int sockfd, void* buf, size_t len, int flags) __attribute__((used));
ssize_t tlibc_recvfrom(int sockfd, void* buf, size_t len,
                       int flags,
                       struct sockaddr * src_addr,
                       socklen_t * addrlen) __attribute__((used));

// clang-format on
void tlibc_internal_explicit_bzero(void *p, size_t n) {
  tlibc_internal_memset(p, 0, n, 0);
}

void tlibc_internal_memset(void *s, char c, size_t n, uint1_t isVolatile) {
  unsigned char *dst = (unsigned char *)s;
  while (n > 0) {
    *dst = (unsigned char)c;
    dst++;
    n--;
  }
}

int tlibc_internal_memcmp(const void *s1, const void *s2, size_t n) {
  if (!n) {
    return 0;
  }
  while (--n && (*(char *)s1 == *(char *)s2)) {
    s1 = (char *)s1 + 1;
    s2 = (char *)s2 + 1;
  }
  return (*((unsigned char *)s1) - *((unsigned char *)s2));
}

void tlibc_internal_memcpy(void *dest, const void *src, size_t n,
                           uint1_t isVolatile) {
  // memcpy(dest, src, n);

  while (n > 0) {
    *(unsigned char *)dest = *(unsigned char *)src;
    dest = ((char *)dest) + 1;
    src = ((char *)src) + 1;
    n--;
  }
}

void tlibc_internal_memmove(void *dest, const void *src, size_t n,
                            uint1_t isVolatile) {
  uint8_t *from = (uint8_t *)src;
  uint8_t *to = (uint8_t *)dest;

  if (from == to || n == 0)
    return;
  if (to > from && to - from < (int)n) {
    /* to overlaps with from */
    /*  <from......>         */
    /*         <to........>  */
    /* copy in reverse, to avoid overwriting from */
    int i;
    for (i = n - 1; i >= 0; i--)
      to[i] = from[i];
    return;
  }
  if (from > to && from - to < (int)n) {
    /* to overlaps with from */
    /*        <from......>   */
    /*  <to........>         */
    /* copy forwards, to avoid overwriting from */
    size_t i;
    for (i = 0; i < n; i++)
      to[i] = from[i];
    return;
  }
  tlibc_internal_memcpy(dest, src, n, isVolatile);
  return;
}

size_t tlibc_internal_strlen(const char *s) {
  size_t len = 0;
  while (*s != '\0') {
    len++;
    s++;
  }
  return len;
}

char *tlibc_internal_strcat(char *destination, const char *source) {
  // make ptr point to the end of destination string
  char *ptr = destination + tlibc_internal_strlen(destination);

  // Appends characters of source to the destination string
  while (*source != '\0')
    *ptr++ = *source++;

  // null terminate destination string
  *ptr = '\0';

  // destination is returned by standard strcat()
  return destination;
}

char *tlibc_internal_strcpy(char *destination, const char *source) {
  // return if no memory is allocated to the destination
  if (destination == NULL)
    return NULL;

  // take a pointer pointing to the beginning of destination string
  char *ptr = destination;

  // copy the C-string pointed by source into the array
  // pointed by destination
  while (*source != '\0') {
    *destination = *source;
    destination++;
    source++;
  }

  // include the terminating null character
  *destination = '\0';

  // destination is returned by standard strcpy()
  return ptr;
}

// Function to implement strcmp function
int tlibc_internal_strcmp(const char *X, const char *Y) {
  while (*X) {
    // if characters differ or end of second string is reached
    if (*X != *Y) {
      break;
    }

    // move to next pair of characters
    X++;
    Y++;
  }
  return *(const unsigned char *)X - *(const unsigned char *)Y;
}

char *tlibc_internal_strstr(const char *str, const char *substr) {
  while (*str) {
    char *Begin = str;
    char *pattern = substr;

    // If first character of sub string match, check for whole string
    while (*str && *pattern && *str == *pattern) {
      str++;
      pattern++;
    }
    // If complete sub string match, return starting address
    if (!*pattern)
      return Begin;

    str = Begin + 1; // Increament main string
  }
  return NULL;
}

ssize_t tlibc_internal_read(int fd, void *buf, size_t count) {
  char tmp[8000];
  int ret = syscall(SYS_read, fd, tmp, count);
  memcpy(buf, tmp, ret);
  return ret;
}

ssize_t tlibc_recv(int sockfd, void *buf, size_t len, int flags) {
  char tmp[8000];
  ssize_t ret = syscall(SYS_recvfrom, sockfd, tmp, len, flags, NULL, NULL);
  memcpy(buf, tmp, ret);
  return ret;
}

ssize_t tlibc_recvfrom(int sockfd, void *buf, size_t len, int flags,
                       struct sockaddr *src_addr, socklen_t *addrlen) {
  char tmp[8000];
  ssize_t ret = syscall(SYS_recvfrom, sockfd, tmp, len, flags, src_addr, addrlen);
  memcpy(buf, tmp, ret);
  return ret;
}
