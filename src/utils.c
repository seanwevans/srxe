// utils.c

#include "utils.h"

// Convert a hexadecimal escape sequence to its integer value
int hex_to_int(const char *hex) {
  int value = 0;
  for (int i = 0; i < 2; i++) { // Convert the two hex digits
    char c = hex[i];
    if (isdigit(c)) {
      value = value * 16 + (c - '0');
    } else if (c >= 'a' && c <= 'f') {
      value = value * 16 + (c - 'a' + 10);
    } else if (c >= 'A' && c <= 'F') {
      value = value * 16 + (c - 'A' + 10);
    }
  }
  return value;
}

// Convert a Unicode escape sequence to its integer value
int unicode_to_int(const char *unicode) {
  int value = 0;
  for (int i = 0; i < 4; i++) { // Convert the four hex digits
    char c = unicode[i];
    if (isdigit(c)) {
      value = value * 16 + (c - '0');
    } else if (c >= 'a' && c <= 'f') {
      value = value * 16 + (c - 'a' + 10);
    } else if (c >= 'A' && c <= 'F') {
      value = value * 16 + (c - 'A' + 10);
    }
  }
  return value;
}

// Check if the character is a whitespace (for \s)
bool is_whitespace(char c) { return isspace(c); }

// Check if a character is in a character class
bool char_in_class(char c, const char *regex, int *class_len) {
  const char *start = regex;
  bool negate = false;
  bool match = false;

  if (*regex == '^') {
    negate = true;
    regex++;
  }

  while (*regex != ']' && *regex != '\0') {
    if (*(regex + 1) == '-' && *(regex + 2) != ']') {
      // Handle range (e.g., a-z)
      if (c >= *regex && c <= *(regex + 2)) {
        match = true;
      }
      regex += 3; // Move past the range
    } else {
      if (c == *regex) {
        match = true;
      }
      regex++;
    }
  }

  *class_len = regex - start + 1; // Length of character class
  return negate ? !match : match;
}

// Case insensitive character comparison
bool char_equals(char c1, char c2, bool case_insensitive) {
  return case_insensitive ? tolower(c1) == tolower(c2) : c1 == c2;
}

// Parse the {min,max} quantifier
void parse_braces(const char *regex, int *min, int *max) {
  *min = 0;
  *max = -1;
  sscanf(regex, "{%d,%d}", min, max);
}
