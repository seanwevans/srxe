// quantifiers.h

#ifndef SRX_QUANTIFIERS_H
#define SRX_QUANTIFIERS_H

#include <stdbool.h>

// Greedy (*)
bool match_star(char c, const char *regex, const char *text,
                bool case_insensitive, bool dot_all, bool multi_line);

// Lazy (*?)
bool match_star_lazy(char c, const char *regex, const char *text,
                     bool case_insensitive, bool dot_all, bool multi_line);

// possessive (*+)
bool match_star_possessive(char c, const char *regex, const char *text,
                           bool case_insensitive, bool dot_all,
                           bool multi_line);

// Lazy Quantifiers for Other Patterns (e.g., +?, {min,max}?)
bool match_lazy_range(char c, const char *regex, const char *text, int min,
                      int max, bool case_insensitive, bool dot_all,
                      bool multi_line);

// Match between min and max occurrences of a character
bool match_range(char c, const char *regex, const char *text, int min, int max,
                 bool case_insensitive, bool dot_all, bool multi_line);

// Match between min occurrences and infinity of a character
bool match_range_infinite(char c, const char *regex, const char *text, int min,
                          bool case_insensitive, bool dot_all, bool multi_line);

#endif // SRX_QUANTIFIERS_H
