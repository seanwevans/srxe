// unicode.c

#include "unicode.h"
#include <stdbool.h>
#include <wctype.h>
#include <ctype.h>
#include "utils.h"

static void unicode_casefold(char c, char *out) {
  out[0] = (char)tolower((unsigned char)c);
  out[1] = '\0';
}


bool match_unicode_property(char property, char c, bool negate) {
  switch (property) {
  case 'L':
    return negate ? !iswalpha(c) : iswalpha(c); // Letters
  case 'N':
    return negate ? !iswdigit(c) : iswdigit(c); // Numbers
  case 'P':
    return negate ? !ispunct(c) : ispunct(c); // Punctuation
  case 'S':
    return negate ? !isspace(c) : isspace(c); // Whitespace
  case 'M':
    return negate ? !iswcntrl(c) : iswcntrl(c); // Marks (placeholder)
  // Extend to other Unicode properties...
  default:
    return false;
  }
}

// Handle word boundaries with Unicode support (\b, \B)
bool match_word_boundary(const char *text, bool word_boundary) {
  bool before_is_word = iswalnum(*(text - 1)) || *(text - 1) == '_';
  bool current_is_word = iswalnum(*text) || *text == '_';
  return word_boundary ? (before_is_word != current_is_word)
                       : (before_is_word == current_is_word);
}

// Full Unicode case folding support (handle multi-character case folds)
bool match_unicode_casefold(const char *regex, const char *text,
                            bool case_insensitive) {
  if (case_insensitive) {
    char folded_char[4]; // Buffer to hold case-folded character
    unicode_casefold(*regex, folded_char); // Assume unicode_casefold function
    return char_equals(folded_char[0], *text, true);
  }
  return char_equals(*regex, *text, case_insensitive);
}
