#include "kstring.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "khash.h"
#include "kobject.h"

struct KString {
  size_t size;
  void* buffer;
};

static void KStringInit(struct KString* str) {}

static void KStringDeinit(struct KString* str) {
  free(str->buffer);
}

static KClass kStringClass = {.init = (KClassInit)&KStringInit,
                              .deinit = (KClassDeinit)&KStringDeinit,
                              .size = sizeof(struct KString)};

KStringRef KStringAllocWithFormat(const char* format, ...) {
  va_list args;
  va_start(args, format);
  KStringRef str = KStringAllocWithFormatV(format, args);
  va_end(args);
  return str;
}

KStringRef KStringAllocWithFormatV(const char* format, va_list args) {
  KStringRef str = KObjectAlloc(&kStringClass);

  if (!str) {
    return NULL;
  }

  va_list args_len;
  va_copy(args_len, args);
  int count = vsnprintf(NULL, 0u, format, args_len);
  va_end(args_len);

  str->buffer = malloc(count + 1);
  str->size = count;

  if (str->buffer) {
    vsnprintf(str->buffer, count + 1, format, args);
  }

  if (!str->buffer) {
    KObjectRelease(str);
    return NULL;
  }

  return str;
}

void KStringRetain(KStringRef str) {
  KObjectRetain(str);
}

void KStringRelease(KStringRef str) {
  KObjectRelease(str);
}

size_t KStringGetLength(KStringRef str) {
  return str->size;
}

const char* KStringGetData(KStringRef str) {
  return str->buffer;
}

size_t KStringGetHash(KStringRef str) {
  KHash hash = 31;
  if (str) {
    hash = KHashCombine(
        KHashCreate((const uint8_t*)KStringGetData(str), KStringGetLength(str)),
        hash);
  }
  return hash;
}

bool KStringIsEqual(KStringRef lhs, KStringRef rhs) {
  if (!lhs || !rhs) {
    return false;
  }
  size_t lhs_len = KStringGetLength(lhs);
  size_t rhs_len = KStringGetLength(rhs);
  if (lhs_len != rhs_len) {
    return false;
  }
  if (lhs_len == 0u) {
    return true;
  }
  return memcmp(KStringGetData(lhs), KStringGetData(rhs), lhs_len) == 0;
}
