// character_classes.c

#include "character_classes.h"

// Handle character class ranges
bool match_char_class(const char *regex, char c, bool case_insensitive) {
  bool negate = (*regex == '^');
  if (negate)
    regex++;
  bool found = false;

  while (*regex != ']' && *regex != '\0') {
    if (*(regex + 1) == '-') {
      char range_start = *regex;
      char range_end = *(regex + 2);
      regex += 3;
      if (char_equals(c, range_start, case_insensitive) ||
          char_equals(c, range_end, case_insensitive) ||
          (c > range_start && c < range_end)) {
        found = true;
      }
    } else {
      if (char_equals(*regex, c, case_insensitive)) {
        found = true;
      }
      regex++;
    }
  }
  return negate ? !found : found;
}

// Handle inline case-folding inside character classes ((?i)[a-z])
bool match_inline_casefold(const char *regex, const char *text,
                           bool case_insensitive) {
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == 'i') {
    return match_char_class(regex + 4, *text, true);
  }
  return match_char_class(regex, *text, case_insensitive);
}

// Multi-Character Character Classes (e.g., [abc|xyz])
bool match_multichar_class(const char *regex, const char *text,
                           bool case_insensitive) {
  bool found = false;
  while (*regex != ']' && *regex != '\0') {
    if (*(regex + 1) == '|') {
      if (char_equals(*regex, *text, case_insensitive)) {
        found = true;
      }
      regex += 2;
    } else if (char_equals(*regex, *text, case_insensitive)) {
      found = true;
      regex++;
    }
  }
  return found;
}

// Intersection and subtraction in character classes
bool match_intersection_class(const char *regex, const char *text,
                              bool case_insensitive) {
  bool negate = false, found = false;
  bool subtracted = false;
  char char_class[256] = {0};
  char excluded_class[256] = {0};

  while (*regex != ']' && *regex != '\0') {
    if (*regex == '^' && regex == text + 1) {
      negate = true;
    } else if (*regex == '[' && *(regex + 1) == '&') {
      regex += 3;
      subtracted = true;
      while (*regex != ']' && *regex != '\0') {
        excluded_class[strlen(excluded_class)] = *regex++;
      }
    } else {
      char_class[strlen(char_class)] = *regex++;
    }
  }

  for (size_t i = 0; i < strlen(char_class); i++) {
    if (char_equals(char_class[i], *text, case_insensitive)) {
      found = true;
    }
  }

  if (subtracted) {
    for (size_t j = 0; j < strlen(excluded_class); j++) {
      if (char_equals(excluded_class[j], *text, case_insensitive)) {
        found = false;
        break;
      }
    }
  }

  return negate ? !found : found;
}

// Shorthand Classes for horizontal (\h) and vertical whitespace (\v)
bool match_shorthand_class(char shorthand, char c) {
  switch (shorthand) {
  case 'h':
    return c == ' ' || c == '\t'; // Horizontal whitespace
  case 'v':
    return c == '\n' || c == '\r'; // Vertical whitespace
  default:
    return false;
  }
}
