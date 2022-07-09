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
