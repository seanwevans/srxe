#include "quantifiers.h"
#include "engine.h"
#include "utils.h"

// Helper to check if character c matches t with options
static bool matches(char c, char t, bool case_insensitive, bool dot_all) {
  if (c == '.') {
    return dot_all || t != '\n';
  }
  return char_equals(c, t, case_insensitive);
}

// Greedy (*) quantifier
bool match_star(char c, const char *regex, const char *text,
                bool case_insensitive, bool dot_all, bool multi_line) {
  const char *t = text;
  while (*t != '\0' && matches(c, *t, case_insensitive, dot_all)) {
    t++;
  }
  do {
    if (match_here(regex, t, case_insensitive, dot_all, multi_line)) {
      return true;
    }
  } while (t-- > text);
  return false;
}

// Lazy (*?) quantifier
bool match_star_lazy(char c, const char *regex, const char *text,
                     bool case_insensitive, bool dot_all, bool multi_line) {
  const char *t = text;
  while (1) {
    if (match_here(regex, t, case_insensitive, dot_all, multi_line)) {
      return true;
    }
    if (*t == '\0' || !matches(c, *t, case_insensitive, dot_all)) {
      return false;
    }
    t++;
  }
}

// Possessive (*+) quantifier (no backtracking)
bool match_star_possessive(char c, const char *regex, const char *text,
                           bool case_insensitive, bool dot_all,
                           bool multi_line) {
  const char *t = text;
  while (*t != '\0' && matches(c, *t, case_insensitive, dot_all)) {
    t++;
  }
  if (*regex == '\0') {
    return *t == '\0';
  }
  return match_here(regex, t, case_insensitive, dot_all, multi_line);
}

// Lazy range quantifier (e.g., +?, {min,max}?)
bool match_lazy_range(char c, const char *regex, const char *text, int min,
                      int max, bool case_insensitive, bool dot_all,
                      bool multi_line) {
  const char *t = text;
  int count = 0;

  while (count < min) {
    if (*t == '\0' || !matches(c, *t, case_insensitive, dot_all)) {
      return false;
    }
    t++;
    count++;
  }

  while (count <= max) {
    if (match_here(regex, t, case_insensitive, dot_all, multi_line)) {
      return true;
    }
    if (count == max || *t == '\0' || !matches(c, *t, case_insensitive, dot_all)) {
      return false;
    }
    t++;
    count++;
  }

  return false;
}

// Greedy range quantifier (e.g., {min,max})
bool match_range(char c, const char *regex, const char *text, int min, int max,
                 bool case_insensitive, bool dot_all, bool multi_line) {
  const char *t = text;
  int count = 0;

  while (count < max && *t != '\0' && matches(c, *t, case_insensitive, dot_all)) {
    t++;
    count++;
  }

  while (count >= min) {
    if (match_here(regex, t, case_insensitive, dot_all, multi_line)) {
      return true;
    }
    t--;
    count--;
  }

  return false;
}

// Range with no upper bound (e.g., {min,})
bool match_range_infinite(char c, const char *regex, const char *text, int min,
                          bool case_insensitive, bool dot_all,
                          bool multi_line) {
  const char *t = text;
  int count = 0;

  while (*t != '\0' && matches(c, *t, case_insensitive, dot_all)) {
    t++;
    count++;
  }

  if (count < min) {
    return false;
  }

  do {
    if (match_here(regex, t, case_insensitive, dot_all, multi_line)) {
      return true;
    }
  } while (t-- > text + min);

  return false;
}

