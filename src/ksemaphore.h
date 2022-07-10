#pragma once

#include "kmacros.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

struct KSemaphore;
typedef struct KSemaphore* KSemaphoreRef;

KSemaphoreRef KSemaphoreAlloc(size_t count);

void KSemaphoreRetain(KSemaphoreRef sema);

void KSemaphoreRelease(KSemaphoreRef sema);

bool KSemaphoreWait(KSemaphoreRef sema);

bool KSemaphoreSignal(KSemaphoreRef sema);

KEXTERN_C_END
