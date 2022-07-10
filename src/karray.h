#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

struct KArray;
typedef struct KArray* KArrayRef;

KArrayRef KArrayAlloc();

bool KArrayAddObject(KArrayRef array, KObjectRef object);

bool KArrayRemoveObjectAtIndex(KArrayRef array, size_t index);

KObjectRef KArrayGetObjectAtIndex(KArrayRef array, size_t index);

size_t KArrayGetLength(KArrayRef array);

void KArrayRetain(KArrayRef array);

void KArrayRelease(KArrayRef array);

size_t KArrayRemoveAllObjects(KArrayRef array);

KEXTERN_C_END
