#pragma once

#include "kmacros.h"
#include "kstring.h"

K_EXTERN_C_BEGIN

//------------------------------------------------------------------------------
/// Represents an OS specific file path.
///
K_DEF_OBJECT(KFilePath);

//------------------------------------------------------------------------------
/// @brief      Create a file path with the given format and arguments.
///
/// @param[in]  format     The format.
/// @param[in]  args       The arguments.
///
/// @return     The new file path reference.
///
K_PRINTF_FORMAT(1, 2)
KFilePathRef KFilePathNewWithFormat(const char* format, ...);

//------------------------------------------------------------------------------
/// @brief      Create a file path with the given string.
///
/// @param[in]  path  The path string.
///
/// @return     The new file path reference.
///
KFilePathRef KFilePathNewWithString(KStringRef path);

//------------------------------------------------------------------------------
/// @brief      Add a new file path component to an existing path.
///
/// @param[in]  path  The path.
/// @param[in]  comp  The component to add.
///
/// @return     If the file path component could be added to the existing path.
///
bool KFilePathAddComponent(KFilePathRef path, KFilePathRef comp);

//------------------------------------------------------------------------------
/// @brief      Check if the path is an absolute path.
///
/// @param[in]  path  The path.
///
/// @return     If the path is absolute.
///
bool KFilePathIsAbsolute(KFilePathRef path);

//------------------------------------------------------------------------------
/// @brief      Get the number of components in the file path.
///
/// @param[in]  path  The path.
///
/// @return     The number of components in the file path.
///
size_t KFilePathCountComponents(KFilePathRef path);

//------------------------------------------------------------------------------
/// @brief      Get a platform specific native string representation of the
///             given file path.
///
/// @param[in]  path  The path.
///
/// @return     The new string reference.
///
KStringRef KFilePathNewStringRepresentation(KFilePathRef path);

//------------------------------------------------------------------------------
/// @brief      Get a string representation of the given file path using the
///             supplied separator.
///
/// @param[in]  path       The path.
/// @param[in]  separator  The separator.
///
/// @return     The new string reference.
///
KStringRef KFilePathNewStringRepresentationWithSeparator(KFilePathRef path,
                                                         char separator);

K_EXTERN_C_END
