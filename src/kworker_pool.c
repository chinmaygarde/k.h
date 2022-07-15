#include "kworker_pool.h"

#include "karray.h"
#include "kassert.h"
#include "kcondition_variable.h"
#include "klist.h"
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
  KListRef tasks;
  bool shutdown;
};
K_IMPL_OBJECT(KWorkerPool);

void KWorkerPoolInit(KWorkerPoolRef pool) {}

void KWorkerPoolDeInit(KWorkerPoolRef pool) {
  K_ASSERT(KWorkerPoolShutdown(pool));
  KConditionVariableRelease(pool->cv);
  KArrayRelease(pool->workers);
  KListRelease(pool->tasks);
}

static bool KWorkerPoolCVPredicate(void* user_data) {
  KWorkerPoolRef pool = (KWorkerPoolRef)user_data;
  return KListGetCount(pool->tasks) == 0 && !pool->shutdown;
}

static void KWorkerPoolWorkerMain(KWorkerPoolRef pool) {
  bool is_running = true;
  while (is_running) {
    K_ASSERT(KConditionVariableCriticalSectionEnter(pool->cv));
    if (pool->shutdown) {
      is_running = false;
    }
    K_ASSERT(KConditionVariableWait(pool->cv, &KWorkerPoolCVPredicate, pool));
    KWorkerPoolTaskRef task = KListRemoveObjectAtIndex(pool->tasks, 0u);
    K_ASSERT(KConditionVariableCriticalSectionExit(pool->cv));
    if (task) {
      KWorkerPoolTaskExecute(task);
      KWorkerPoolTaskRelease(task);
    }
  }
}

KWorkerPoolRef KWorkerPoolNew(size_t worker_count) {
  if (worker_count == 0) {
    return NULL;
  }
  KWorkerPoolRef pool = KWorkerPoolAlloc();
  if (!pool) {
    return NULL;
  }
  pool->cv = KConditionVariableNew();
  pool->workers = KArrayNew();
  pool->tasks = KListNew();
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
                         KWorkerPoolTask task_proc,
                         KObjectRef user_data) {
  if (!pool || !task_proc || !user_data) {
    return false;
  }

  KWorkerPoolTaskRef task = KWorkerPoolTaskAlloc();
  task->task = task_proc;
  task->user_data = user_data;
  KObjectRetain(task->user_data);
  K_ASSERT(KConditionVariableCriticalSectionEnter(pool->cv));
  K_ASSERT(KListAddObject(pool->tasks, task));
  K_ASSERT(KConditionVariableCriticalSectionExit(pool->cv));
  KWorkerPoolTaskRelease(task);

  K_ASSERT(KConditionVariableNotifyOne(pool->cv));
  return true;
}

static bool KWorkerJoinIterator(KObjectRef obj, size_t index, void* user_data) {
  KThreadJoin((KThreadRef)obj);
  return true;
}

bool KWorkerPoolShutdown(KWorkerPoolRef pool) {
  if (!pool) {
    return false;
  }
  K_ASSERT(KConditionVariableCriticalSectionEnter(pool->cv));
  pool->shutdown = true;
  K_ASSERT(KConditionVariableCriticalSectionExit(pool->cv));
  K_ASSERT(KConditionVariableNotifyAll(pool->cv));
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
