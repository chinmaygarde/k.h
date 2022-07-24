#pragma once

#include "kmacros.h"
#include "kobject.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// A thread of execution. These are OS threads (i.e, not green threads).
///
K_DEF_OBJECT(KThread);

typedef void (*KThreadProc)(void*);

KThreadRef KThreadNew(KThreadProc proc, void* user_data);

void KThreadJoin(KThreadRef thread);

size_t KThreadGetHardwareConcurrency();

void KThreadSleepSeconds(double seconds);

uint64_t KThreadGetCurrentID();

K_EXTERN_C_END
