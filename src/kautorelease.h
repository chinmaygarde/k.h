#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

bool KAutoreleasePoolAddObject(KObjectRef obj);

KObjectRef KAutoreleaseMove(KObjectRef obj);

size_t KAutoreleasePoolPop();

KEXTERN_C_END
