// escapes.c

#include "escapes.h"
#include "posix.h"
#include "utils.h"
#include "unicode.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

// Handle escape sequences like \s, \d, \w, \D, \S, \W, \b, \B, \xHH, \uHHHH
bool match_escape(char esc, const char *regex, const char *text,
                  bool case_insensitive) {
  if (esc == '[' && regex[1] == ':') {
    // Handle POSIX class format [:class_name:]
    return match_posix_bracket_expression(regex, text);
  }

  if (esc == 'x') {
    // Handle \xHH escape sequence (hexadecimal)
    int hex_value =
        hex_to_int(regex);           // Convert the next two characters from hex
    return *text == (char)hex_value; // Match the character based on hex value
  } else if (esc == 'u') {
    // Handle \uHHHH escape sequence (Unicode)
    int unicode_value = unicode_to_int(
        regex); // Convert the next four characters from hex (unicode)
    return *text ==
           (char)unicode_value; // Match the character based on Unicode value
  }

  if (esc == 'p' || esc == 'P') {
    // Handle \p{...} and \P{...} for Unicode properties
    char property =
        regex[0]; // Next character after p/P is the Unicode property
    return match_unicode_property(property, *text, (esc == 'P'));
  }

  // Handle POSIX character classes [:alpha:], [:digit:], etc.
  if (esc == '[' && regex[1] == ':') {
    // We expect a POSIX class format [:class_name:], for example [:alpha:]
    char class_name[10];
    sscanf(regex + 2, "%9[^:]:]", class_name);
    return match_posix_class(class_name, *text);
  }

  // Handle other escape sequences
  switch (esc) {
  case 's':
    return isspace(*text);
  case 'S':
    return !isspace(*text);
  case 'd':
    return isdigit(*text);
  case 'D':
    return !isdigit(*text);
  case 'w':
    return isalnum(*text) || *text == '_';
  case 'W':
    return !(isalnum(*text) || *text == '_');
  case 'b': // Word boundary
    return (isalnum(*text) || *text == '_') !=
           (isalnum(*(text - 1)) || *(text - 1) == '_');
  case 'B': // Not a word boundary
    return (isalnum(*text) || *text == '_') ==
           (isalnum(*(text - 1)) || *(text - 1) == '_');
  case '\\':
    return *text == '\\'; // Literal backslash
  case '(':
    return *text == '('; // Literal parenthesis
  case ')':
    return *text == ')'; // Literal parenthesis
  default:
    return false;
  }
}