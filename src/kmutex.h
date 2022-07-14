#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KMutex);

KMutexRef KMutexNew();

bool KMutexLock(KMutexRef mutex);

bool KMutexUnlock(KMutexRef mutex);

K_EXTERN_C_END
