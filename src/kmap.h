#pragma once

#include <stdbool.h>
#include "kmacros.h"
#include "kobject.h"

KEXTERN_C_BEGIN

struct KMap;
typedef struct KMap* KMapRef;

typedef size_t (*KMapKeyHash)(KObjectRef obj);
typedef size_t (*KMapKeyEqual)(KObjectRef lhs, KObjectRef rhs);

KMapRef KMapAlloc(KMapKeyHash hash, KMapKeyEqual equal);

void KMapRetain(KMapRef map);

void KMapRelease(KMapRef map);

bool KMapSet(KMapRef map, KObjectRef key, KObjectRef value);

KObjectRef KMapGet(KMapRef map, KObjectRef key);

bool KMapContains(KMapRef map, KObjectRef key);

bool KMapRemove(KMapRef map, KObjectRef key);

KEXTERN_C_END
