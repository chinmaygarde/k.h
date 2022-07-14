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

#define K_NO_RETURN __attribute__((noreturn))
#define K_THREAD_LOCAL _Thread_local

#endif

#define K_DEF_OBJECT(obj)             \
  struct obj;                         \
  typedef struct obj* obj##Ref;       \
  KEXTERN_C_BEGIN                     \
  void obj##Retain(obj##Ref object);  \
  void obj##Release(obj##Ref object); \
  KEXTERN_C_END

#define K_IMPL_OBJECT(obj)                  \
  void obj##Retain(obj##Ref object) {       \
    KObjectRetain(object);                  \
  }                                         \
  void obj##Release(obj##Ref object) {      \
    KObjectRelease(object);                 \
  }                                         \
  void obj##Init(obj##Ref obj);             \
  void obj##DeInit(obj##Ref obj);           \
  static KClass obj##ClassDef = {           \
      .init = (KClassInit)&obj##Init,       \
      .deinit = (KClassDeinit)&obj##DeInit, \
      .size = sizeof(struct obj),           \
  };                                        \
  obj##Ref obj##AllocPriv() {               \
    return KObjectAlloc(&obj##ClassDef);    \
  }
