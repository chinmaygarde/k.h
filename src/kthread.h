#pragma once

#include "kmacros.h"
#include "kobject.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// A thread of execution. These are OS threads (i.e, not green threads).
///
K_DEF_OBJECT(KThread);

//------------------------------------------------------------------------------
/// A callback made on the newly created thread.
///
typedef void (*KThreadProc)(void*);

//------------------------------------------------------------------------------
/// @brief      Creates a new thread of execution. Once created the supplied
///             proc is invoked on thaht thread.
///
/// @param[in]  proc       The proc.
/// @param      user_data  The user data supplied to the proc.
///
/// @return     The new thread reference.
///
KThreadRef KThreadNew(KThreadProc proc, void* user_data);

//------------------------------------------------------------------------------
/// @brief      Joins the thread.
///
/// @param[in]  thread  The thread
///
void KThreadJoin(KThreadRef thread);

//------------------------------------------------------------------------------
/// @brief      Gets the logical core count on the device. This can be used to
///             determine the optimum number of workers in a work pool.
///
/// @return     The logical core count on the device.
///
size_t KThreadGetHardwareConcurrency();

//------------------------------------------------------------------------------
/// @brief      Makes the current thread sleep for the given number of seconds.
///
/// @param[in]  seconds  The seconds
///
void KThreadSleepSeconds(double seconds);

//------------------------------------------------------------------------------
/// @brief      Get the thread ID of the current thread.
///
/// @return     The thread ID.
///
uint64_t KThreadGetCurrentID();

K_EXTERN_C_END
