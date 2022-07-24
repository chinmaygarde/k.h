#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// Represents a contiguous allocation.
///
K_DEF_OBJECT(KAllocation);

//------------------------------------------------------------------------------
/// @brief      Create a new allocation.
///
/// @return     The new allocation reference.
///
KAllocationRef KAllocationNew();

//------------------------------------------------------------------------------
/// @brief      Update the length of the allocation. A new allocation may be
///             created and the existing data moved it. If the new size is
///             greater than the current size, the contents of the newly
///             allocated region is undefined.
///
///             If the truncation fails, the existing allocation remained
///             unchanged. This may happen in case of resource exhaustion.
///
/// @param[in]  allocation  The allocation
/// @param[in]  size        The new size
///
/// @return     If the truncation was successful.
///
bool KAllocationTruncate(KAllocationRef allocation, size_t size);

//------------------------------------------------------------------------------
/// @brief      Get the current size of the allocation.
///
/// @param[in]  allocation  The allocation
///
/// @return     The size of the allocation.
///
size_t KAllocationGetSize(KAllocationRef allocation);

//------------------------------------------------------------------------------
/// @brief      Get a pointer to the contiguous buffer for the allocation.
///
/// @param[in]  allocation  The allocation
///
/// @return     A pointer to the buffer for the allocation.
///
void* KAllocationGetBuffer(KAllocationRef allocation);

//------------------------------------------------------------------------------
/// @brief      Get the next power-of-two size given the current size. If the
///             size is already a power of two, the same value is returned.
///
/// @param[in]  size  The size
///
/// @return     The next power-of-two size.
///
uint32_t KAllocationNextPowerOfTwoSize(uint32_t size);

K_EXTERN_C_END
