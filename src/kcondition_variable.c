#include "kcondition_variable.h"

#include "kobject.h"
#include "kplatform.h"

#include <pthread.h>

K_IMPL_OBJECT(KConditionVariable);

struct KConditionVariable {
  pthread_mutex_t mutex;
  pthread_cond_t cv;
  bool is_valid;
};

void KConditionVariableInit(KConditionVariableRef cv) {}

void KConditionVariableDeInit(KConditionVariableRef cv) {
  if (cv->is_valid) {
    pthread_mutex_destroy(&cv->mutex);
    pthread_cond_destroy(&cv->cv);
  }
}

static KClass KConditionVariableClass = {
    .init = (KClassInit)(&KConditionVariableInit),
    .deinit = (KClassInit)(&KConditionVariableDeInit),
    .size = sizeof(struct KConditionVariable),
};

KConditionVariableRef KConditionVariableAlloc() {
  KConditionVariableRef cv = KObjectAlloc(&KConditionVariableClass);
  if (!cv) {
    return NULL;
  }

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  if (pthread_mutex_init(&mutex, NULL) != 0) {
    KConditionVariableRelease(cv);
    return NULL;
  }
  pthread_cond_t cv_handle = PTHREAD_COND_INITIALIZER;
  if (pthread_cond_init(&cv_handle, NULL) != 0) {
    pthread_mutex_destroy(&mutex);
    KConditionVariableRelease(cv);
    return NULL;
  }
  cv->mutex = mutex;
  cv->cv = cv_handle;
  cv->is_valid = true;

  return cv;
}

bool KConditionVariableWait(KConditionVariableRef cv,
                            KConditionVariablePredicate pred,
                            void* user_data) {
  if (!cv || !pred) {
    return false;
  }

  if (!cv->is_valid) {
    return false;
  }

  if (pthread_mutex_lock(&cv->mutex) != 0) {
    return false;
  }

  bool success = false;

  do {
    if (!pred(user_data)) {
      success = true;
      break;
    }
  } while (pthread_cond_wait(&cv->cv, &cv->mutex) == 0);

  if (pthread_mutex_unlock(&cv->mutex) != 0) {
    return false;
  }
  return success;
}

bool KConditionVariableNotifyOne(KConditionVariableRef cv) {
  if (!cv) {
    return false;
  }
  if (!cv->is_valid) {
    return false;
  }
  return pthread_cond_signal(&cv->cv) == 0;
}

bool KConditionVariableNotifyAll(KConditionVariableRef cv) {
  if (!cv) {
    return false;
  }
  if (!cv->is_valid) {
    return false;
  }
  return pthread_cond_broadcast(&cv->cv) == 0;
}
