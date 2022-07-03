#pragma once

#include <stddef.h>
#include "kmacros.h"

KEXTERN_C_BEGIN

struct KString;
typedef struct KString* KStringRef;

KPRINTF_FORMAT(1, 2)
KStringRef KStringAllocWithFormat(const char* format, ...);

void KStringRetain(KStringRef);

void KStringRelease(KStringRef);

size_t KStringGetLength(KStringRef);

const char* KStringGetData(KStringRef);

KEXTERN_C_END
