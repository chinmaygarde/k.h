#pragma once

#include "kplatform.h"

#ifdef __cplusplus

#define KEXTERN_C_BEGIN extern "C" {
#define KEXTERN_C_END }

#else  // __cplusplus

#define KEXTERN_C_BEGIN
#define KEXTERN_C_END

#endif  // __cplusplus

#if defined(__GNUC__) || defined(__clang__)
#define KPRINTF_FORMAT(format_number, args_number) \
  __attribute__((format(printf, format_number, args_number)))
#else  // defined(__GNUC__) || defined(__clang__)
#define KPRINTF_FORMAT(format_number, args_number)
#endif  // defined(__GNUC__) || defined(__clang__)

#if K_OS_WIN

#define K_NO_RETURN __declspec(noreturn)
#define K_THREAD_LOCAL __declspec(thread)

#else

#define K_NO_RETURN _Noreturn
#define K_THREAD_LOCAL _Thread_local

#endif
