#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KSemaphore);

KSemaphoreRef KSemaphoreNew(size_t count);

bool KSemaphoreWait(KSemaphoreRef semaphore);

bool KSemaphoreSignal(KSemaphoreRef semaphore);

K_EXTERN_C_END
