#pragma once

#include "kmacros.h"

#if defined(_WIN32)

#define K_OS_WIN 1

#elif defined(__APPLE__)

#define K_OS_DARWIN 1

#elif defined(__linux__)

#define K_OS_LINUX 1

#else

#error "Unknown Platform."

#endif

#if K_OS_DARWIN || K_OS_LINUX

#include <errno.h>

#define K_HANDLE_EINTR(x)                                   \
  ({                                                        \
    int eintr_wrapper_result;                               \
    do {                                                    \
      eintr_wrapper_result = (x);                           \
    } while (eintr_wrapper_result == -1 && errno == EINTR); \
    eintr_wrapper_result;                                   \
  })

#endif  // K_OS_DARWIN || K_OS_LINUX
