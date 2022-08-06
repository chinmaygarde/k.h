#pragma once

#include "kmacros.h"
#include "kobject.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KSet)

typedef size_t (*KSetHash)(KObjectRef object);
typedef bool (*KSetEqual)(KObjectRef lhs, KObjectRef rhs);

KSetRef KSetNew(KSetHash hash, KSetEqual equal);

bool KSetInsert(KSetRef set, KObjectRef object);

bool KSetErase(KSetRef set, KObjectRef object);

bool KSetContains(KSetRef set, KObjectRef object);

size_t KSetGetCount(KSetRef set);

bool KSetClear(KSetRef set);

K_EXTERN_C_END
