#pragma once

#include "kmacros.h"
#include "kobject.h"
#include "ktypes.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// A heap-allocated string object.
///
K_DEF_OBJECT(KString);

//------------------------------------------------------------------------------
/// @brief      A new string object with the given format and arguments.
///
/// @param[in]  format     The format string.
/// @param[in]  arguments  The arguments for the format string.
///
/// @return     The new string reference.
///
K_PRINTF_FORMAT(1, 2)
KStringRef KStringNewWithFormat(const char* format, ...);

//------------------------------------------------------------------------------
/// @brief      A new string object with the given format and arguments.
///
/// @param[in]  format     The format string.
/// @param[in]  arguments  The arguments for the format string.
///
/// @return     The new string reference.
///
KStringRef KStringNewWithFormatV(const char* format, va_list arguments);

//------------------------------------------------------------------------------
/// @brief      A new string object with the contents copied from the data
///             provided.
///
/// @param[in]  data   The data buffer to copy the string information from.
/// @param[in]  count  The number of bytes copied from the data.
///
/// @return     The new string reference.
///
KStringRef KStringNewWithData(const char* data, size_t count);

//------------------------------------------------------------------------------
/// @brief      A new empty string.
///
/// @return     The new string reference.
///
KStringRef KStringNew();

//------------------------------------------------------------------------------
/// @brief      A new copy of the provided string.
///
/// @param[in]  string  The string to copy from.
///
/// @return     The new string reference.
///
KStringRef KStringCopy(KStringRef string);

//------------------------------------------------------------------------------
/// @brief      Append the suffix to the given string. The appended string
///             information is copied.
///
/// @param[in]  string  The string to append to string.
/// @param[in]  suffix  The string to append.
///
/// @return     If the string could be appended. May fail due to allocation
///             failures.
///
bool KStringAppend(KStringRef string, KStringRef suffix);

//------------------------------------------------------------------------------
/// @brief      Get the length of the string in bytes.
///
/// @param[in]  string  The string.
///
/// @return     The length of the string in bytes.
///
size_t KStringGetLength(KStringRef string);

//------------------------------------------------------------------------------
/// @brief      Get pointer to a contiguous buffer containing the string data.
///
/// @param[in]  string  The string.
///
/// @return     The contiguous buffer containing string data.
///
const char* KStringGetData(KStringRef string);

//------------------------------------------------------------------------------
/// @brief      Get the string hash.
///
/// @param[in]  string  The string.
///
/// @return     The string hash.
///
size_t KStringGetHash(KStringRef string);

//------------------------------------------------------------------------------
/// @brief      Compare if the two string contain identical data.
///
/// @param[in]  lhs   The left hand side string.
/// @param[in]  rhs   The right hand side string.
///
/// @return     If the two strings are identical.
///
bool KStringIsEqual(KStringRef lhs, KStringRef rhs);

K_EXTERN_C_END
