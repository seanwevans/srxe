// escapes.h

#ifndef SRX_ESCAPES_H
#define SRX_ESCAPES_H

#include <stdbool.h>

// Handle escape sequences like \s, \d, \w, \D, \S, \W, \b, \B, \xHH, \uHHHH
bool match_escape(char esc, const char *regex, const char *text,
                  bool case_insensitive);

#endif // SRX_ESCAPES_H
