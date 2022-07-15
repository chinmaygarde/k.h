#include "kcountdown_latch.h"

#include "kassert.h"
#include "kcondition_variable.h"
#include "kobject.h"

struct KCountdownLatch {
  KConditionVariableRef cv;
  size_t count;
};

K_IMPL_OBJECT(KCountdownLatch);

void KCountdownLatchInit(KCountdownLatchRef cdl) {}

void KCountdownLatchDeInit(KCountdownLatchRef cdl) {
  KConditionVariableRelease(cdl->cv);
}

KCountdownLatchRef KCountdownLatchNew(size_t count) {
  if (count == 0) {
    return NULL;
  }
  KCountdownLatchRef cdl = KCountdownLatchAlloc();
  cdl->cv = KConditionVariableNew();
  cdl->count = count;
  K_ASSERT(cdl->cv);
  return cdl;
}

bool KCountDownLatchCountDown(KCountdownLatchRef cdl) {
  if (!cdl) {
    return false;
  }
  bool should_signal = false;
  K_ASSERT(KConditionVariableCriticalSectionEnter(cdl->cv));
  switch (cdl->count) {
    case 0:
      break;
    case 1:
      cdl->count--;
      should_signal = true;
      break;
    default:
      cdl->count--;
      break;
  }
  K_ASSERT(KConditionVariableCriticalSectionExit(cdl->cv));
  if (should_signal) {
    K_ASSERT(KConditionVariableNotifyAll(cdl->cv));
  }
  return true;
}

static bool KCountDownLatchCVPredicate(void* user_data) {
  return ((KCountdownLatchRef)user_data)->count != 0;
}

bool KCountDownLatchWait(KCountdownLatchRef cdl) {
  if (!cdl) {
    return false;
  }
  K_ASSERT(KConditionVariableCriticalSectionEnter(cdl->cv));
  K_ASSERT(KConditionVariableWait(cdl->cv, &KCountDownLatchCVPredicate, cdl));
  K_ASSERT(KConditionVariableCriticalSectionExit(cdl->cv));
  return true;
}
