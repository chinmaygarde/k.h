#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

K_DEF_OBJECT(KSemaphore);

KSemaphoreRef KSemaphoreNew(size_t count);

bool KSemaphoreWait(KSemaphoreRef semaphore);

bool KSemaphoreSignal(KSemaphoreRef semaphore);

KEXTERN_C_END
