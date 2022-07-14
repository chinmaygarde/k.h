#include "kcondition_variable.h"

#include "kobject.h"
#include "kplatform.h"

#if K_OS_WIN
#include <Windows.h>
#else  // K_OS_WIN
#include <pthread.h>
#endif  // K_OS_WIN

K_IMPL_OBJECT(KConditionVariable);

struct KConditionVariable {
#if K_OS_WIN
  CRITICAL_SECTION mutex;
  CONDITION_VARIABLE cv;
#else   // K_OS_WIN
  pthread_mutex_t mutex;
  pthread_cond_t cv;
  bool is_valid;
#endif  // K_OS_WIN
};

void KConditionVariableInit(KConditionVariableRef cv) {}

void KConditionVariableDeInit(KConditionVariableRef cv) {
#if K_OS_WIN
  DeleteCriticalSection(&cv->mutex);
#else   // K_OS_WIN
  if (cv->is_valid) {
    pthread_mutex_destroy(&cv->mutex);
    pthread_cond_destroy(&cv->cv);
  }
#endif  // K_OS_WIN
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

#if K_OS_WIN
  InitializeCriticalSection(&cv->mutex);
  InitializeConditionVariable(&cv->cv);
#else   // K_OS_WIN
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
#endif  // K_OS_WIN

  return cv;
}

bool KConditionVariableWait(KConditionVariableRef cv,
                            KConditionVariablePredicate pred,
                            void* user_data) {
  if (!cv || !pred) {
    return false;
  }

#if K_OS_WIN
  EnterCriticalSection(&cv->mutex);

  bool success = false;
  do {
    if (!pred(user_data)) {
      success = true;
      break;
    }
  } while (SleepConditionVariableCS(&cv->cv, &cv->mutex, INFINITE) != 0);

  LeaveCriticalSection(&cv->mutex);

  return success;
#else   // K_OS_WIN
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
#endif  // K_OS_WIN
}

bool KConditionVariableNotifyOne(KConditionVariableRef cv) {
  if (!cv) {
    return false;
  }

#if K_OS_WIN
  WakeConditionVariable(&cv->cv);
  return true;
#else   // K_OS_WIN
  if (!cv->is_valid) {
    return false;
  }
  return pthread_cond_signal(&cv->cv) == 0;
#endif  // K_OS_WIN
}

bool KConditionVariableNotifyAll(KConditionVariableRef cv) {
  if (!cv) {
    return false;
  }

#if K_OS_WIN
  WakeAllConditionVariable(&cv->cv);
  return true;
#else   // K_OS_WIN
  if (!cv->is_valid) {
    return false;
  }
  return pthread_cond_broadcast(&cv->cv) == 0;
#endif  // K_OS_WIN
}
