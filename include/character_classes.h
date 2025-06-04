// character_classes.h

#ifndef SRX_CHARACTER_CLASSES_H
#define SRX_CHARACTER_CLASSES_H

// Handle character class ranges
bool match_char_class(const char *regex, char c, bool case_insensitive);

// Handle inline case-folding inside character classes ((?i)[a-z])
bool match_inline_casefold(const char *regex, const char *text,
                           bool case_insensitive);

// Multi-Character Character Classes (e.g., [abc|xyz])
bool match_multichar_class(const char *regex, const char *text,
                           bool case_insensitive);

// Intersection and subtraction in character classes
bool match_intersection_class(const char *regex, const char *text,
                              bool case_insensitive);

// Shorthand Classes for horizontal (\h) and vertical whitespace (\v)
bool match_shorthand_class(char shorthand, char c);

#endif // SRX_CHARACTER_CLASSES_H
