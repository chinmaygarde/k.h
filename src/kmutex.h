#pragma once

#include "kmacros.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

struct KMutex;
typedef struct KMutex* KMutexRef;

KMutexRef KMutexAlloc();

void KMutexRetain(KMutexRef);

void KMutexRelease(KMutexRef);

bool KMutexLock(KMutexRef);

bool KMutexUnlock(KMutexRef);

KEXTERN_C_END
