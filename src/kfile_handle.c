#include "kfile_handle.h"

#include <fcntl.h>
#include <unistd.h>

#include "kassert.h"

struct KFileHandle {
  int handle;
  bool closed;
};

K_IMPL_OBJECT(KFileHandle);

void KFileHandleInit(KFileHandleRef handle) {}

void KFileHandleDeInit(KFileHandleRef handle) {
  K_ASSERT(KFileHandleClose(handle));
}

KFileHandleRef KFileHandleNew(KFilePathRef path, KFilePermission permission) {
  if (!path) {
    return NULL;
  }
  KStringRef string = KFilePathNewStringRepresentation(path);
  if (!string) {
    return NULL;
  }

  int flags = O_CLOEXEC;
  switch (permission) {
    case kFilePermissionReadOnly:
      flags |= O_RDONLY;
      break;
    case kFilePermissionWriteOnly:
      flags |= O_WRONLY;
      break;
    case kFilePermissionReadWrite:
      flags |= O_RDWR;
      break;
  }
  int raw_handle = K_HANDLE_EINTR(open(KStringGetData(string), flags));
  KStringRelease(string);
  if (raw_handle == -1) {
    return NULL;
  }
  KFileHandleRef handle = KFileHandleAlloc();
  handle->handle = raw_handle;
  return handle;
}

bool KFileHandleClose(KFileHandleRef handle) {
  if (!handle) {
    return false;
  }
  if (handle->closed) {
    return true;
  }
  return K_HANDLE_EINTR(close(handle->handle)) == 0;
}
