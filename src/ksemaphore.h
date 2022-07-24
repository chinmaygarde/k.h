#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// A counting semaphore.
///
K_DEF_OBJECT(KSemaphore);

//------------------------------------------------------------------------------
/// @brief      Create a new semaphore initialized to the given count.
///
/// @param[in]  count  The count
///
/// @return     The new semaphore reference. Can fail due to resource
///             exhaustion.
///
KSemaphoreRef KSemaphoreNew(size_t count);

//------------------------------------------------------------------------------
/// @brief      Decrement the semaphore. If the count is current zero, blocks
///             the calling thread till the count is incremented by a
///             corresponding signal on the semaphore object.
///
/// @param[in]  semaphore  The semaphore
///
/// @return     If the wait was successful.
///
bool KSemaphoreWait(KSemaphoreRef semaphore);

//------------------------------------------------------------------------------
/// @brief      Increment the semaphore. If doing so increment the count to be
///             greater than zero, any threads waiting via `KSemaphoreWait` are
///             unblocked.
///
/// @param[in]  semaphore  The semaphore
///
/// @return     If the signal was successful.
///
bool KSemaphoreSignal(KSemaphoreRef semaphore);

K_EXTERN_C_END
