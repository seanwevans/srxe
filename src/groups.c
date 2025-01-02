// groups.c

#include "groups.h"

// Handle alternation (|)
bool match_alternation(const char *regex, const char *text,
                       bool case_insensitive, bool dot_all, bool multi_line) {
  const char *alternation_start = regex;
  while (*regex != '\0' && *regex != '|') {
    regex++;
  }
  // Try left side first
  if (match_here(alternation_start, text, case_insensitive, dot_all,
                 multi_line)) {
    return true;
  }
  // If there is a right-hand side to check, try it
  if (*regex == '|') {
    return match_here(regex + 1, text, case_insensitive, dot_all, multi_line);
  }
  return false;
}

// Handle atomic groups (?>...)
bool match_atomic_group(const char *regex, const char *text,
                        bool case_insensitive, bool dot_all, bool multi_line) {
  int nesting_level = 1;
  while (*regex != '\0' && nesting_level > 0) {
    if (*regex == '(') {
      nesting_level++;
    } else if (*regex == ')') {
      nesting_level--;
    }
    if (nesting_level > 0 &&
        !match_here(regex, text, case_insensitive, dot_all, multi_line)) {
      return false;
    }
    regex++;
  }
  return true;
}

// Structure to store captured groups
typedef struct {
  char *captured_text[MAX_GROUPS];
  int group_lengths[MAX_GROUPS];
} group_capture;

// Subroutine Calls ((?1), (?&name))
bool match_subroutine(const char *regex, const char *text,
                      bool case_insensitive, bool dot_all, bool multi_line,
                      int group_num, group_capture *captures) {
  // Match the previous captured group (group_num)
  if (group_num > 0 && group_num < MAX_GROUPS) {
    if (strncmp(text, captures->captured_text[group_num],
                captures->group_lengths[group_num]) == 0) {
      return match_here(regex, text + captures->group_lengths[group_num],
                        case_insensitive, dot_all, multi_line);
    }
  }
  return false;
}

// Handle conditional matching ((?(condition)yes|no))
bool match_conditional(const char *regex, const char *text, bool condition_met,
                       bool case_insensitive, bool dot_all, bool multi_line) {
  if (condition_met) {
    return match_here(regex, text, case_insensitive, dot_all, multi_line);
  } else {
    int nesting_level = 0;
    // Skip to the '|' character, respecting nested groups
    while (*regex != '\0') {
      if (*regex == '(') {
        nesting_level++;
      } else if (*regex == ')') {
        nesting_level--;
      } else if (*regex == '|' && nesting_level == 0) {
        return match_here(regex + 1, text, case_insensitive, dot_all,
                          multi_line);
      }
      regex++;
    }
  }
  return false;
}

// Handle anchors within groups (^, $, \A, \Z, \z)
bool match_anchor(const char *regex, const char *text, const char *start,
                  bool case_insensitive, bool dot_all, bool multi_line) {
  if (*regex == '^' && text == start) {
    return match_here(regex + 1, text, case_insensitive, dot_all, multi_line);
  }
  if (*regex == '$' && *text == '\0') {
    return true;
  }
  return false;
}

// Non-capturing group handling
bool match_non_capturing_group(const char *regex, const char *text,
                               bool case_insensitive, bool dot_all,
                               bool multi_line) {
  while (*regex != ')' && *regex != '\0') {
    if (!match_here(regex, text, case_insensitive, dot_all, multi_line)) {
      return false;
    }
    regex++;
  }
  return true;
}
