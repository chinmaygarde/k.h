#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

bool KAutoreleasePoolAddObject(KObjectRef obj);

KObjectRef KAutoreleaseMove(KObjectRef obj);

size_t KAutoreleasePoolPop();

K_EXTERN_C_END
