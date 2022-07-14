#include "ktime.h"

#include "kassert.h"
#include "kplatform.h"

#if K_OS_WIN
#include <Windows.h>
#else  // K_OS_WIN
#include <time.h>
#endif  // K_OS_WIN

double KTimeGetCurrentSeconds() {
#if K_OS_WIN
  LARGE_INTEGER counter;
  LARGE_INTEGER frequency;
  BOOL counter_result = QueryPerformanceCounter(&counter);
  BOOL frequency_result = QueryPerformanceFrequency(&frequency);
  K_ASSERT(counter_result && frequency_result);
  return (double)counter.QuadPart / frequency.QuadPart;
#else   // K_OS_WIN
  struct timespec ts = {};
  int result = clock_gettime(CLOCK_MONOTONIC, &ts);
  K_ASSERT(result == 0);
  uint64_t ns = ts.tv_sec * 1e9 + ts.tv_nsec;
  return (double)ns / 1e9;
#endif  // K_OS_WIN
}
