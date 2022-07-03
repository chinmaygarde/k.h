#pragma once

#include <stdbool.h>
#include "kmacros.h"
#include "kobject.h"

KEXTERN_C_BEGIN

bool KAutoreleasePoolAddObject(KObjectRef obj);

KObjectRef KAutoreleaseMove(KObjectRef obj);

size_t KAutoreleasePoolPop();

KEXTERN_C_END
