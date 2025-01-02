// lookaround.c

#include "lookaround.h"

// Handle variable-length lookbehind assertions ((?<=...) or (?<!...))
bool match_lookbehind(const char *regex, const char *text, bool positive,
                      bool case_insensitive, bool dot_all, bool multi_line) {
  const char *start = text;
  // We will attempt to match from the start of the string to the current text
  // position
  while (start <= text) {
    if (match_here(regex, start, case_insensitive, dot_all, multi_line)) {
      return positive; // Return true if positive lookbehind, false otherwise
    }
    start++;
  }
  return !positive; // Return false if positive lookbehind, true otherwise
}

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
                      int lookbehind_length) {
  const char *lookbehind_start =
      (text >= lookbehind_length) ? text - lookbehind_length : NULL;

  if (lookbehind_start && match_here(regex, lookbehind_start, case_insensitive,
                                     dot_all, multi_line)) {
    return positive ? true : false;
  }

  return !positive; // Return false for positive lookbehind if no match
}
