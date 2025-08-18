// lookaround.c

#include "lookaround.h"
#include <stdbool.h>
#include <stddef.h>
#include "engine.h"

// Handle lookahead assertions ((?=...), (?!...))
bool match_lookahead(const char *regex, const char *text, bool positive,
                     bool case_insensitive, bool dot_all, bool multi_line) {
  bool result = match_here(regex, text, case_insensitive, dot_all, multi_line);
  return positive ? result : !result;
}

// Handle fixed-length or variable-length lookbehind assertions with boundary
// check
bool match_lookbehind(const char *regex, const char *text, bool positive,
                      bool case_insensitive, bool dot_all, bool multi_line,
                      int lookbehind_length, const char *start) {
  const char *lookbehind_start = text - lookbehind_length;
  if (lookbehind_start < start) {
    return !positive;
  }
  bool result = match_here(regex, lookbehind_start, case_insensitive, dot_all,
                           multi_line);
  return positive ? result : !result;
}
