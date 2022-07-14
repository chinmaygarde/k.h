#pragma once

#include "kmacros.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

typedef uint64_t KHash;

KHash KHashCombine(KHash a, KHash b);

KHash KHashCreate(const uint8_t* data, size_t length);

K_EXTERN_C_END
