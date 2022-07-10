#pragma once

#include "kmacros.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

struct KString;
typedef struct KString* KStringRef;

KPRINTF_FORMAT(1, 2)
KStringRef KStringAllocWithFormat(const char* format, ...);

KStringRef KStringAllocWithFormatV(const char* format, va_list args);

void KStringRetain(KStringRef);

void KStringRelease(KStringRef);

size_t KStringGetLength(KStringRef);

const char* KStringGetData(KStringRef);

size_t KStringGetHash(KStringRef str);

bool KStringIsEqual(KStringRef lhs, KStringRef rhs);

KEXTERN_C_END
