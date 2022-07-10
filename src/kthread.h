#pragma once

#include "kmacros.h"
#include "kobject.h"

KEXTERN_C_BEGIN

struct KThread;
typedef struct KThread* KThreadRef;

typedef void (*KThreadProc)(void*);

KThreadRef KThreadAlloc(KThreadProc proc, void* user_data);

void KThreadRetain(KThreadRef);

void KThreadRelease(KThreadRef);

void KThreadJoin(KThreadRef);

KEXTERN_C_END
