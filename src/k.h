#pragma once

//------------------------------------------------------------------------------
/// Base include.
///
#include "kmacros.h"
#include "ktypes.h"

//------------------------------------------------------------------------------
/// Platform detection.
///
#include "kplatform.h"

//------------------------------------------------------------------------------
/// Thread-safe reference counted objects.
///
#include "kobject.h"

//------------------------------------------------------------------------------
/// Synchronization
///
#include "kcondition_variable.h"
#include "kcountdown_latch.h"
#include "kmutex.h"
#include "ksemaphore.h"

//------------------------------------------------------------------------------
/// Threading
///
#include "kthread.h"
#include "ktime.h"
#include "kworker_pool.h"

//------------------------------------------------------------------------------
/// Filesystem
///
#include "kfile_handle.h"
#include "kfile_path.h"

//------------------------------------------------------------------------------
/// Logging
///
#include "klogging.h"

//------------------------------------------------------------------------------
/// Utilities
///
#include "kallocation.h"
#include "kassert.h"
#include "kautorelease.h"
#include "khash.h"
#include "kmapping.h"

//------------------------------------------------------------------------------
/// Collections
///
#include "karray.h"
#include "klist.h"
#include "kmap.h"
#include "kset.h"
#include "kstring.h"
