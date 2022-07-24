#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// An array of object stored in a contiguous buffer.
///
K_DEF_OBJECT(KArray);

//------------------------------------------------------------------------------
/// @brief      Creates a new array.
///
/// @return     The new array reference.
///
KArrayRef KArrayNew();

//------------------------------------------------------------------------------
/// @brief      Append an item to the end of the array. The array owns a
///             reference to the item.
///
/// @param[in]  array   The array.
/// @param[in]  object  The object.
///
/// @return     If the object could be appended to the end of the array.
///
bool KArrayAddObject(KArrayRef array, KObjectRef object);

//------------------------------------------------------------------------------
/// @brief      Remove an item at the given index. Bounds checks are performed
///             before removing the item at the given index.
///
/// @param[in]  array  The array.
/// @param[in]  index  The index of the item to remove.
///
/// @return     If the item could be removed from the array.
///
bool KArrayRemoveObjectAtIndex(KArrayRef array, size_t index);

//------------------------------------------------------------------------------
/// @brief      Get the object at the specified index in the array. If the index
///             is out of bounds, NULL is returned.
///
/// @param[in]  array  The array.
/// @param[in]  index  The index of the item to fetch.
///
/// @return     The object reference.
///
KObjectRef KArrayGetObjectAtIndex(KArrayRef array, size_t index);

//------------------------------------------------------------------------------
/// @brief      Get the number of items in the array.
///
/// @param[in]  array  The array.
///
/// @return     The number of items in the array.
///
size_t KArrayGetLength(KArrayRef array);

//------------------------------------------------------------------------------
/// @brief      Remove all items from the array. The array relinquishes
///             references to all objects.
///
/// @param[in]  array  The array
///
/// @return     The number of items removed from the array.
///
size_t KArrayRemoveAllObjects(KArrayRef array);

//------------------------------------------------------------------------------
/// The callback invoked by `KArrayIterate` on each item in the array. The index
/// of the item is specified along with the user data passed to the function
/// call.
///
/// Further iteration is terminated if the method return false.
///
typedef bool (*KArrayIterator)(KObjectRef obj, size_t index, void* user_data);

//------------------------------------------------------------------------------
/// @brief      Iterate the elements of the array.
///
/// @warning    It is an error to mutate the array while iterating over its
///             elements.
///
/// @param[in]  array      The array
/// @param[in]  iterator   The iterator
/// @param      user_data  The user data
///
/// @return     The number of elements iterated over in the array. If the
///             iterator returns true for each element in the array, this is the
///             size of the array.
///
size_t KArrayIterate(KArrayRef array, KArrayIterator iterator, void* user_data);

K_EXTERN_C_END
