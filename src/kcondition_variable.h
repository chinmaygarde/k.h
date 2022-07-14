#pragma once

#include "kmacros.h"
#include "ktypes.h"

KEXTERN_C_BEGIN

K_DEF_OBJECT(KConditionVariable);

typedef bool (*KConditionVariablePredicate)(void* user_data);

KConditionVariableRef KConditionVariableAlloc();

bool KConditionVariableWait(KConditionVariableRef cv,
                            KConditionVariablePredicate pred,
                            void* user_data);

bool KConditionVariableNotifyOne(KConditionVariableRef cv);

bool KConditionVariableNotifyAll(KConditionVariableRef cv);

KEXTERN_C_END
