// utils.h

#ifndef SRX_UTILS_H
#define SRX_UTILS_H

// Convert a hexadecimal escape sequence to its integer value
int hex_to_int(const char *hex);

// Convert a Unicode escape sequence to its integer value
int unicode_to_int(const char *unicode);

// Check if the character is whitespace
bool is_whitespace(char c);

// Check if a character is in a character class
bool char_in_class(char c, const char *regex, int *class_len);

// Case insensitive character comparison
bool char_equals(char c1, char c2, bool case_insensitive);

// Parse the {min,max} quantifier
void parse_braces(const char *regex, int *min, int *max);

#endif // SRX_UTILS_H
