// unicode.h

#ifndef SRX_UNICODE_H
#define SRX_UNICODE_H


// Check if a character matches a Unicode property (like \p{L})
bool match_unicode_property(char property, char c, bool negate);

// Handle word boundaries with Unicode support (\b, \B)
bool match_word_boundary(const char *text, bool word_boundary);

// Case Folding for Unicode (e.g., (?i))
bool match_unicode_casefold(const char *regex, const char *text,
                            bool case_insensitive);

#endif // SRX_UNICODE_H