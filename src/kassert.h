#pragma once

#include "klogging.h"
#include "kmacros.h"

//------------------------------------------------------------------------------
/// @brief      Asserts that the condition is true. If not, log a message and
///             abort the process.
///
/// @param      condition     The condition to verify.
///
/// @return     { description_of_the_return_value }
///
#define K_ASSERT(condition)                             \
  {                                                     \
    if (!(condition)) {                                 \
      K_LOG_ERROR("Assertion failed '" #condition "'"); \
      KAbort();                                         \
    }                                                   \
  }

#ifndef NDEBUG
#define K_DASSERT K_ASSERT
#else
#define K_DASSERT(...)
#endif

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// @brief      Abort the process.
///
K_NO_RETURN void KAbort();

K_EXTERN_C_END
