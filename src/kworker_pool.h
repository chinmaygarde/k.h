#pragma once

#include "kmacros.h"
#include "kobject.h"

KEXTERN_C_BEGIN

K_DEF_OBJECT(KWorkerPool);

typedef void (*KWorkerPoolTask)(KObjectRef user_data);

KWorkerPoolRef KWorkerPoolAlloc(size_t worker_count);

bool KWorkerPoolPostTask(KWorkerPoolRef pool,
                         KWorkerPoolTask task,
                         KObjectRef user_data);

bool KWorkerPoolShutdown(KWorkerPoolRef pool);

size_t KWorkerPoolGetWorkerCount(KWorkerPoolRef pool);

KEXTERN_C_END
