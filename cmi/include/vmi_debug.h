#ifndef _VMI_DEBUG_H_
#define _VMI_DEBUG_H_
#include <stdio.h>

#ifdef CMI_DBG
#define dbg_printf(fmt, ...)                                                   \
  do {                                                                         \
      fprintf(stderr, fmt, __VA_ARGS__);                                       \
  } while (0)
#else
#define dbg_printf(fmt, ...) {}

#endif

#ifdef CMI_DBG
#define cmi_dbg(...) fprintf(__VA_ARGS__)
#else
#define cmi_dbg(...)                                                           \
  {}
#endif


#endif
