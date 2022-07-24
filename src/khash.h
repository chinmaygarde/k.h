#pragma once

#include "kmacros.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

typedef uint64_t KHash;

//------------------------------------------------------------------------------
/// @brief      Combines to hash values.
///
/// @param[in]  a     The first hash value.
/// @param[in]  b     The second hash value.
///
/// @return     The new hash value.
///
KHash KHashCombine(KHash a, KHash b);

//------------------------------------------------------------------------------
/// @brief      Hash the given data.
///
/// @param[in]  data    The data.
/// @param[in]  length  The length of data in bytes.
///
/// @return     The hash value.
///
KHash KHashCreate(const uint8_t* data, size_t length);

K_EXTERN_C_END
