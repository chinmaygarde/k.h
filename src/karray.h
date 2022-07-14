#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

K_DEF_OBJECT(KArray);

KArrayRef KArrayNew();

bool KArrayAddObject(KArrayRef array, KObjectRef object);

bool KArrayRemoveObjectAtIndex(KArrayRef array, size_t index);

KObjectRef KArrayGetObjectAtIndex(KArrayRef array, size_t index);

size_t KArrayGetLength(KArrayRef array);

size_t KArrayRemoveAllObjects(KArrayRef array);

typedef bool (*KArrayIterator)(KObjectRef obj, size_t index, void* user_data);
size_t KArrayIterate(KArrayRef array, KArrayIterator iterator, void* user_data);

KEXTERN_C_END
