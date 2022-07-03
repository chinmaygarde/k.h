#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kmacros.h"

KEXTERN_C_BEGIN

struct KAllocation;
typedef struct KAllocation* KAllocationRef;

KAllocationRef KAllocationAlloc();

void KAllocationRetain(KAllocationRef allocation);

void KAllocationRelease(KAllocationRef allocation);

bool KAllocationTruncate(KAllocationRef allocation, size_t size);

size_t KAllocationGetSize(KAllocationRef allocation);

void* KAllocationGetBuffer(KAllocationRef allocation);

uint32_t KAllocationNextPowerOfTwoSize(uint32_t size);

KEXTERN_C_END
