// posix.h

#ifndef SRX_POSIX_H
#define SRX_POSIX_H

// Check if a character matches a POSIX character class
bool match_posix_class(const char *class_name, char c);

// Handle POSIX bracket expressions inside character classes
bool match_posix_bracket_expression(const char *regex, const char *text);

#endif // SRX_POSIX_H
