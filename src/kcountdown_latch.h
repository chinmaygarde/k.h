#pragma once

#include "kmacros.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// A synchronization utility that waits for a certain number of jobs to be
/// finished before a waiting thread can continue.
///
K_DEF_OBJECT(KCountdownLatch);

//------------------------------------------------------------------------------
/// @brief      A new count down latch initialized to the given count.
///
/// @param[in]  count  The count.
///
/// @return     The new countdown latch reference.
///
KCountdownLatchRef KCountdownLatchNew(size_t count);

//------------------------------------------------------------------------------
/// @brief      Decrement the count by 1. If the count hits zero, any waiting
///             threads are awoken.
///
/// @param[in]  latch   The latch.
///
/// @return     If the count was successfully decremented.
///
bool KCountDownLatchCountDown(KCountdownLatchRef latch);

//------------------------------------------------------------------------------
/// @brief      Wait for the count to hit zero. If the count is zero, no wait is
///             performed.
///
/// @param[in]  latch  The latch
///
/// @return     If the wait was successfully performed.
///
bool KCountDownLatchWait(KCountdownLatchRef latch);

K_EXTERN_C_END
