// posix.c

#include "posix.h"

bool isascii(unsigned char c) { return c < 128; }

bool isblank(unsigned char c) { return c == ' ' || c == '\t'; }

typedef struct {
  const char *class_name;
  bool (*match_fn)(char);
} PosixClass;

PosixClass posix_classes[] = {
    {"alnum", isalnum},  // alphanumeric
    {"alpha", isalpha},  // alphabetic
    {"ascii", isascii},  // ascii
    {"blank", isblank},  // space or tab
    {"cntrl", iscntrl},  // control character
    {"digit", isdigit},  // digit
    {"graph", isgraph},  // visible
    {"lower", islower},  // lowercase
    {"print", isprint},  // printable
    {"punct", ispunct},  // punctuation
    {"space", isspace},  // whitespace
    {"upper", isupper},  // uppercase
    {"xdigit", isxdigit} // hexadecimal digit
};

// Function to match a character against a POSIX character class
bool match_posix_class(const char *class_name, char c) {
  for (int i = 0; i < sizeof(posix_classes) / sizeof(PosixClass); i++) {
    if (strcmp(posix_classes[i].class_name, class_name) == 0) {
      return posix_classes[i].match_fn(c);
    }
  }

  return false;
}

// Function to handle POSIX bracket expressions inside character classes
bool match_posix_bracket_expression(const char *regex, const char *text) {
  char class_name[64];
  if (sscanf(regex, "[:%63[^:]:]", class_name) == 1) {
    return match_posix_class(class_name, *text);
  }

  return false;
}