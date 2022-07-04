#pragma once

#include <stddef.h>
#include <stdint.h>
#include "kmacros.h"

KEXTERN_C_BEGIN

typedef uint64_t KHash;

KHash KHashCombine(KHash a, KHash b);

KHash KHashCreate(const uint8_t* data, size_t length);

KEXTERN_C_END
