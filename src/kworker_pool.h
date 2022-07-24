#pragma once

#include "kmacros.h"
#include "kobject.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// A worker pool containing a fixed number of threads specified upfront. The
/// threads are joined when the worker pool is collected.
///
K_DEF_OBJECT(KWorkerPool);

//------------------------------------------------------------------------------
/// A callback invoked on a worker thread once a worker is available. The user
/// data parameter is the object passed to the `KWorkerPoolPostTask` method.
///
typedef void (*KWorkerPoolTask)(KObjectRef user_data);

//------------------------------------------------------------------------------
/// @brief      Create a new worker pool with the specified number of threads.
///             The threads are created upfront.
///
/// @param[in]  worker_count  The worker count
///
/// @return     The worker pool reference or NULL on error.
///
KWorkerPoolRef KWorkerPoolNew(size_t worker_count);

//------------------------------------------------------------------------------
/// @brief      Post a task to the worker pool. This task will be executed on an
///             available worker. While posting a task to a worker is serial,
///             their execution is concurrent.
///
/// @param[in]  pool       The pool
/// @param[in]  task       The task
/// @param[in]  user_data  The user data. This object is retained by the pool
///                        and released once the task has been executed.
///
/// @return     If the task could be posted to the pool.
///
bool KWorkerPoolPostTask(KWorkerPoolRef pool,
                         KWorkerPoolTask task,
                         KObjectRef user_data);

//------------------------------------------------------------------------------
/// @brief      Shut down the pool and join all threads.
///
/// @param[in]  pool  The pool
///
/// @return     If the pool could be shut down and the worker threads joined.
///
bool KWorkerPoolShutdown(KWorkerPoolRef pool);

//------------------------------------------------------------------------------
/// @brief      Get the number of threads in the worker pool.
///
/// @param[in]  pool  The pool
///
/// @return     The number of threads in the pool. Returns 0 if the pool has
///             been shutdown.
///
size_t KWorkerPoolGetWorkerCount(KWorkerPoolRef pool);

K_EXTERN_C_END
