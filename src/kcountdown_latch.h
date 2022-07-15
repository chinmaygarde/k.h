#pragma once

#include "kmacros.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

K_DEF_OBJECT(KCountdownLatch);

KCountdownLatchRef KCountdownLatchNew(size_t count);

bool KCountDownLatchCountDown(KCountdownLatchRef cdl);

bool KCountDownLatchWait(KCountdownLatchRef cdl);

K_EXTERN_C_END
