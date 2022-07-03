#include "kstring.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
  KStringRef str = KObjectAlloc(&kStringClass);

  if (!str) {
    return NULL;
  }

  va_list args;
  va_start(args, format);

  int count = vsnprintf(NULL, 0u, format, args);
  str->buffer = malloc(count + 1);
  str->size = count;

  if (str->buffer) {
    vsnprintf(str->buffer, count + 1, format, args);
  }

  va_end(args);

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
