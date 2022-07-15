#include "kfile_path.h"

#include "kassert.h"
#include "klist.h"
#include "kobject.h"
#include "kplatform.h"

struct KFilePath {
  KListRef components;
  bool is_absolute;
};

K_IMPL_OBJECT(KFilePath);

void KFilePathInit(KFilePathRef path) {
  path->components = KListNew();
  K_ASSERT(path->components);
}

void KFilePathDeInit(KFilePathRef path) {
  KListRelease(path->components);
}

KFilePathRef KFilePathNewWithFormat(const char* format, ...) {
  va_list args;
  va_start(args, format);
  KStringRef string = KStringNewWithFormatV(format, args);
  va_end(args);
  KFilePathRef path = KFilePathNewWithString(string);
  KStringRelease(string);
  return path;
}

KFilePathRef KFilePathNewWithString(KStringRef str) {
  if (!str) {
    return NULL;
  }

  KFilePathRef path = KFilePathAlloc();
  K_ASSERT(path);

  const char* buffer = KStringGetData(str);
  size_t start = 0;
  size_t len = KStringGetLength(str);
  for (size_t i = 0; i < len; i++) {
    const bool is_separator = (buffer[i] == '/' || buffer[i] == '\\');
    if (i == 0 && is_separator) {
      path->is_absolute = true;
    }
    if (!is_separator) {
      continue;
    }
    size_t count = i - start;
    if (count > 0) {
      KStringRef comp = KStringNewWithData(buffer + start, count);
      K_ASSERT(KListAddObject(path->components, comp));
      KStringRelease(comp);
    }
    start = i + 1;
  }
  if (start < len) {
    KStringRef comp = KStringNewWithData(buffer + start, len - start);
    K_ASSERT(KListAddObject(path->components, comp));
    KStringRelease(comp);
  }
  return path;
}

bool KFilePathAddComponent(KFilePathRef path, KFilePathRef comp) {
  if (!path) {
    return false;
  }
  if (!comp) {
    return true;
  }
  if (KFilePathIsAbsolute(comp)) {
    return false;
  }
  for (size_t i = 0, count = KListGetCount(comp->components); i < count; i++) {
    KStringRef str = KListGetObjectAtIndex(comp->components, i);
    K_ASSERT(KListAddObject(path->components, str));
  }
  return true;
}

bool KFilePathIsAbsolute(KFilePathRef path) {
  return path ? path->is_absolute : false;
}

KStringRef KFilePathNewStringRepresentation(KFilePathRef path) {
  static const char kPathSeparator =
#if K_OS_WIN
      '\\'
#else   // K_OS_WIN
      '/'
#endif  // K_OS_WIN
      ;
  return KFilePathNewStringRepresentationWithSeparator(path, kPathSeparator);
}

KStringRef KFilePathNewStringRepresentationWithSeparator(KFilePathRef path,
                                                         char sep_char) {
  if (!path) {
    return NULL;
  }
  KStringRef sep = KStringNewWithFormat("%c", sep_char);
  KStringRef result = path->is_absolute ? KStringCopy(sep) : KStringNew();
  for (size_t i = 0, count = KListGetCount(path->components); i < count; i++) {
    KStringRef comp = KListGetObjectAtIndex(path->components, i);
    K_ASSERT(KStringAppend(result, comp));
    if (i != (count - 1)) {
      K_ASSERT(KStringAppend(result, sep));
    }
  }
  KStringRelease(sep);
  return result;
}

size_t KFilePathCountComponents(KFilePathRef path) {
  if (!path) {
    return 0u;
  }
  return KListGetCount(path->components);
}
