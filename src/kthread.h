#pragma once

#include "kmacros.h"
#include "kobject.h"

KEXTERN_C_BEGIN

K_DEF_OBJECT(KThread);

typedef void (*KThreadProc)(void*);

KThreadRef KThreadNew(KThreadProc proc, void* user_data);

void KThreadJoin(KThreadRef thread);

size_t KThreadGetHardwareConcurrency();

void KThreadSleepSeconds(double seconds);

KEXTERN_C_END
