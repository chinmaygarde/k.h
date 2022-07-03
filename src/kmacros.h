#pragma once

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
