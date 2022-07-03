#include "klogging.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "kstring.h"

#define K_ANSI_RED "\x1b[31m"
#define K_ANSI_GREEN "\x1b[32m"
#define K_ANSI_YELLOW "\x1b[33m"
#define K_ANSI_BLUE "\x1b[34m"
#define K_ANSI_MAGENTA "\x1b[35m"
#define K_ANSI_CYAN "\x1b[36m"
#define K_ANSI_RESET "\x1b[0m"

#define K_FORMAT_STRING "%s\n"

#define K_LOG_FORMAT_INFO K_ANSI_GREEN K_FORMAT_STRING K_ANSI_RESET
#define K_LOG_FORMAT_WARNING K_ANSI_YELLOW K_FORMAT_STRING K_ANSI_RESET
#define K_LOG_FORMAT_ERROR K_ANSI_RED K_FORMAT_STRING K_ANSI_RESET

void KLogString(KLogLevel level, KStringRef str) {
  if (!str) {
    return;
  }
  const char* format_str = K_FORMAT_STRING;
  static bool tty_checked = false;
  static bool is_tty = false;
  if (!tty_checked) {
    is_tty = isatty(STDOUT_FILENO) == 1;
    tty_checked = true;
  }
  if (is_tty) {
    switch (level) {
      case KLogLevelInfo:
        format_str = K_LOG_FORMAT_INFO;
        break;
      case KLogLevelWarning:
        format_str = K_LOG_FORMAT_WARNING;
        break;
      case KLogLevelError:
        format_str = K_LOG_FORMAT_ERROR;
        break;
    }
  }
  printf(format_str, KStringGetData(str));
}

void KLog(KLogLevel level, const char* format, ...) {
  va_list args;
  va_start(args, format);
  KStringRef str = KStringAllocWithFormatV(format, args);
  va_end(args);
  KLogString(level, str);
  KStringRelease(str);
}

void KLogFileLine(KLogLevel level,
                  const char* file,
                  int line,
                  const char* format,
                  ...) {
  va_list args;
  va_start(args, format);
  KStringRef str = KStringAllocWithFormatV(format, args);
  va_end(args);
  KLog(level, "%s:%d %s", file, line, KStringGetData(str));
  KStringRelease(str);
}
