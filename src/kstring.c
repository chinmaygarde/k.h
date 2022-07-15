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

static void KStringDeInit(struct KString* str) {
  free(str->buffer);
}

K_IMPL_OBJECT(KString);

KStringRef KStringNewWithFormat(const char* format, ...) {
  va_list args;
  va_start(args, format);
  KStringRef str = KStringNewWithFormatV(format, args);
  va_end(args);
  return str;
}

KStringRef KStringNewWithFormatV(const char* format, va_list args) {
  KStringRef str = KStringAlloc();

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

KStringRef KStringNewWithData(const char* data, size_t count) {
  KStringRef str = KStringAlloc();
  if (!str) {
    return NULL;
  }
  str->size = count;
  str->buffer = malloc(count);
  if (count == 0 && !str->buffer) {
    KStringRelease(str);
    return NULL;
  }
  memcpy(str->buffer, data, count);
  return str;
}

KStringRef KStringNew() {
  return KStringNewWithData(NULL, 0);
}

KStringRef KStringCopy(KStringRef string) {
  if (!string) {
    return NULL;
  }
  return KStringNewWithData(string->buffer, string->size);
}

bool KStringAppend(KStringRef string, KStringRef suffix) {
  if (!string || !suffix) {
    return false;
  }

  void* realloced = realloc(string->buffer, string->size + suffix->size + 1);
  if (!realloced) {
    return false;
  }

  string->buffer = realloced;
  memmove(string->buffer + string->size, suffix->buffer, suffix->size);
  string->size += suffix->size;
  if (string->size != 0) {
    ((char*)string->buffer)[string->size] = '\0';
  }
  return true;
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
  if (lhs == rhs) {
    return true;
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
