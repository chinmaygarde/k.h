#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

K_DEF_OBJECT(KString);

KPRINTF_FORMAT(1, 2)
KStringRef KStringNewWithFormat(const char* format, ...);

KStringRef KStringNewWithFormatV(const char* format, va_list args);

size_t KStringGetLength(KStringRef string);

const char* KStringGetData(KStringRef string);

size_t KStringGetHash(KStringRef string);

bool KStringIsEqual(KStringRef lhs, KStringRef rhs);

KEXTERN_C_END
