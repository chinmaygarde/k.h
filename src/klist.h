#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// A linked list of objects.
///
K_DEF_OBJECT(KList);

//------------------------------------------------------------------------------
/// @brief      Create an empty list of objects.
///
/// @return     The new list reference.
///
KListRef KListNew();

//------------------------------------------------------------------------------
/// @brief      Append an item to the end of the list. The list owns a reference
///             to the object.
///
/// @param[in]  list  The list.
/// @param[in]  obj   The object.
///
/// @return     If the object could be appended to the end of the list.
///
bool KListAddObject(KListRef list, KObjectRef obj);

//------------------------------------------------------------------------------
/// @brief      Remove an item at the specified index in the list. The reference
///             the item removed at the given index is transferred to the
///             caller. If the index is out of bounds, NULL is returned.
///
/// @warning    After the caller is done with the object removed from the list,
///             it must release that reference.
///
/// @param[in]  list   The list.
/// @param[in]  index  The index.
///
/// @return     The object reference of the item that was removed. This
///             reference is now owned by the caller.
///
KObjectRef KListRemoveObjectAtIndex(KListRef list, size_t index);

//------------------------------------------------------------------------------
/// @brief      Get a reference to the object at the given index in the list.
///             There is no transfer of ownership of the object.
///
/// @param[in]  list   The list.
/// @param[in]  index  The index.
///
/// @return     The object reference at the given index.
///
KObjectRef KListGetObjectAtIndex(KListRef list, size_t index);

//------------------------------------------------------------------------------
/// @brief      Get if the object is present in the list.
///
/// @param[in]  list  The list.
/// @param[in]  obj   The object.
///
/// @return     If the object is present in the list.
///
bool KListContainsObject(KListRef list, KObjectRef obj);

//------------------------------------------------------------------------------
/// @brief      Remove all objects from the list. The list release references to
///             all removed objects in the list.
///
/// @param[in]  list  The list.
///
/// @return     The number of objects removed from the list.
///
size_t KListRemoveAllObjects(KListRef list);

//------------------------------------------------------------------------------
/// @brief      Get the number of items in the list.
///
/// @param[in]  list  The list.
///
/// @return     The number of items in the list.
///
size_t KListGetCount(KListRef list);

//------------------------------------------------------------------------------
/// A callback invoked `KListIterate` for each item in the list. Along with the
/// index, the user data supplied as an argument to the function is also passed
/// along.
///
/// Returning `false` from this callback terminates further iteration.
///
typedef bool (*KListIterator)(KObjectRef obj, size_t index, void* user_data);

//------------------------------------------------------------------------------
/// @brief      Iterate over the items in the list.
///
/// @param[in]  list       The list.
/// @param[in]  iterator   The iterator.
/// @param      user_data  The user data.
///
/// @return     The number of items iterated over. If the iterator always
///             returns true, this return the length of the list.
///
size_t KListIterate(KListRef list, KListIterator iterator, void* user_data);

K_EXTERN_C_END
