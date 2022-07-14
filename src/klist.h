#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KList);

KListRef KListNew();

bool KListAddObject(KListRef list, KObjectRef obj);

KObjectRef KListRemoveObjectAtIndex(KListRef list, size_t index);

KObjectRef KListGetObjectAtIndex(KListRef list, size_t index);

bool KListContainsObject(KListRef list, KObjectRef obj);

size_t KListRemoveAllObjects(KListRef list);

size_t KListGetCount(KListRef list);

typedef bool (*KListIterator)(KObjectRef obj, size_t index, void* user_data);
size_t KListIterate(KListRef list, KListIterator iterator, void* user_data);

K_EXTERN_C_END
