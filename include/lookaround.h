// lookaround.h

#ifndef SRX_LOOKAROUND_H
#define SRX_LOOKAROUND_H

#include <stdbool.h>

// Handle fixed-length lookbehind assertions ((?<=...) or (?<!...))
bool match_lookbehind(const char *regex, const char *text, bool positive,
                      bool case_insensitive);

// Handle lookahead assertions ((?=...), (?!...))
bool match_lookahead(const char *regex, const char *text, bool positive,
                     bool case_insensitive, bool dot_all, bool multi_line);

// Handle lookbehind assertions ((?<=...), (?<!...))
bool match_lookbehind(const char *regex, const char *text, bool positive,
                      bool case_insensitive, bool dot_all, bool multi_line,
                      int lookbehind_length);

#endif // SRX_LOOKAROUND_H
