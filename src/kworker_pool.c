#include "kworker_pool.h"

#include "karray.h"
#include "kassert.h"
#include "kcondition_variable.h"
#include "klogging.h"
#include "kobject.h"
#include "kthread.h"

//------------------------------------------------------------------------------
/// Worker Pool Task Implementation.
///

struct KWorkerPoolTask {
  KWorkerPoolTask task;
  KObjectRef user_data;
};

K_DEF_OBJECT(KWorkerPoolTask);
K_IMPL_OBJECT(KWorkerPoolTask);

void KWorkerPoolTaskInit(KWorkerPoolTaskRef task) {}

void KWorkerPoolTaskDeInit(KWorkerPoolTaskRef task) {
  if (task->task) {
    K_LOG_WARNING("Worker pool task dropped without execution.");
  }
  KObjectRelease(task->user_data);
}

void KWorkerPoolTaskExecute(KWorkerPoolTaskRef task) {
  task->task(task->user_data);
  KObjectRelease(task->user_data);
  task->task = NULL;
  task->user_data = NULL;
}

//------------------------------------------------------------------------------
/// @brief      Worker Pool Implementation.
///

struct KWorkerPool {
  KConditionVariableRef cv;
  KArrayRef workers;
  KArrayRef tasks;
  bool shutdown;
};
K_IMPL_OBJECT(KWorkerPool);

void KWorkerPoolInit(KWorkerPoolRef pool) {}

void KWorkerPoolDeInit(KWorkerPoolRef pool) {
  K_ASSERT(KWorkerPoolShutdown(pool));
  KConditionVariableRelease(pool->cv);
  KArrayRelease(pool->workers);
  KArrayRelease(pool->tasks);
}

static void KWorkerPoolWorkerMain(KWorkerPoolRef pool) {}

KWorkerPoolRef KWorkerPoolNew(size_t worker_count) {
  KWorkerPoolRef pool = KWorkerPoolAlloc();
  if (!pool) {
    return NULL;
  }
  pool->cv = KConditionVariableNew();
  pool->workers = KArrayNew();
  pool->tasks = KArrayNew();
  for (size_t i = 0; i < worker_count; i++) {
    KThreadRef thread = KThreadNew((KThreadProc)&KWorkerPoolWorkerMain, pool);
    KArrayAddObject(pool->workers, thread);
    KThreadRelease(thread);
  }
  if (!pool->cv || !pool->workers || !pool->tasks ||
      KArrayGetLength(pool->workers) != worker_count) {
    KWorkerPoolRelease(pool);
    return NULL;
  }
  return pool;
}

bool KWorkerPoolPostTask(KWorkerPoolRef pool,
                         KWorkerPoolTask task,
                         KObjectRef user_data);

static bool KWorkerJoinIterator(KObjectRef obj, size_t index, void* user_data) {
  KThreadJoin((KThreadRef)obj);
  return true;
}

bool KWorkerPoolShutdown(KWorkerPoolRef pool) {
  if (!pool) {
    return false;
  }
  if (!KConditionVariableCriticalSectionEnter(pool->cv)) {
    return false;
  }
  pool->shutdown = true;
  if (!KConditionVariableCriticalSectionExit(pool->cv)) {
    return false;
  }
  if (!KConditionVariableNotifyAll(pool->cv)) {
    return false;
  }
  KArrayIterate(pool->workers, &KWorkerJoinIterator, NULL);
  KArrayRemoveAllObjects(pool->workers);
  return true;
}

size_t KWorkerPoolGetWorkerCount(KWorkerPoolRef pool) {
  if (!pool) {
    return 0u;
  }
  return KArrayGetLength(pool->workers);
}
