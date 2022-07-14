#pragma once

#include "kmacros.h"
#include "kstring.h"

K_EXTERN_C_BEGIN

typedef enum KLogLevel {
  KLogLevelInfo,
  KLogLevelWarning,
  KLogLevelError,
} KLogLevel;

K_PRINTF_FORMAT(2, 3)
void KLog(KLogLevel level, const char* format, ...);

void KLogString(KLogLevel level, KStringRef string);

K_PRINTF_FORMAT(4, 5)
void KLogFileLine(KLogLevel level,
                  const char* file,
                  int line,
                  const char* format,
                  ...);

#define K_LOG_INFO(format, ...) \
  KLogFileLine(KLogLevelInfo, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define K_LOG_WARNING(format, ...) \
  KLogFileLine(KLogLevelWarning, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define K_LOG_ERROR(format, ...) \
  KLogFileLine(KLogLevelError, __FILE__, __LINE__, format, ##__VA_ARGS__)

#ifndef NDEBUG
#define K_DLOG_INFO K_LOG_INFO
#define K_DLOG_WARNING K_LOG_WARNING
#define K_DLOG_ERROR K_LOG_ERROR
#else  // NDEBUG
#define K_DLOG_INFO(...)
#define K_DLOG_WARNING(...)
#define K_DLOG_ERROR(...)
#endif  // NDEBUG

K_EXTERN_C_END
