#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

K_DEF_OBJECT(KMutex);

KMutexRef KMutexNew();

bool KMutexLock(KMutexRef mutex);

bool KMutexUnlock(KMutexRef mutex);

KEXTERN_C_END
