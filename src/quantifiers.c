#include "quantifiers.h"

// Function to handle group matching with alternation support
bool match_group(const char *regex, const char *text, bool case_insensitive) {
  const char *group_start = regex;
  const char *group_end = regex;
  int parentheses_count = 0;

  // Find the matching closing parenthesis and handle nested groups
  while (*group_end != '\0') {
    if (*group_end == '(') {
      parentheses_count++;
    } else if (*group_end == ')') {
      parentheses_count--;
      if (parentheses_count == 0) {
        break;
      }
    }
    group_end++;
  }

  // Handle alternation within the group
  return match_alternation(group_start, group_end, text, case_insensitive);
}

// Function to handle alternation within groups (e.g., a|b|c)
bool match_alternation(const char *start, const char *end, const char *text,
                       bool case_insensitive) {
  const char *alt_start = start;
  const char *alt_end = start;

  while (alt_end <= end && *alt_end != '\0') {
    // Look for alternation symbol '|'
    if (*alt_end == '|') {
      // Try matching the current alternation pattern
      if (match_here(alt_start, text, case_insensitive, false, false)) {
        return true;
      }
      alt_start = alt_end + 1; // Move to the next alternative
    }
    alt_end++;
  }

  // Try the last pattern after the last '|'
  return match_here(alt_start, text, case_insensitive, false, false);
}

// Extended function to match more complex patterns (e.g., character classes,
// groups)
bool match_pattern(const char *pattern, const char *text,
                   bool case_insensitive) {
  if (*pattern == '[') {
    // Handle character classes [a-z], [^a-z], etc.
    return match_char_class(pattern, *text, case_insensitive);
  } else if (*pattern == '(') {
    // Handle groups (abc), (a|b|c), etc.
    return match_group(pattern, text, case_insensitive);
  } else {
    // Default to single character match
    return char_equals(*text, *pattern, case_insensitive);
  }
}

// Generalized function to handle greedy (*) and lazy (*?) quantifiers
bool match_star_general(const char *pattern, const char *regex,
                        const char *text, bool case_insensitive, bool dot_all,
                        bool multi_line, bool lazy) {
  do {
    if (match_here(regex, text, case_insensitive, dot_all, multi_line)) {
      return true;
    }
    if (lazy) {
      break; // Lazy match, stop as soon as there's a match
    }
  } while (*text != '\0' && match_pattern(pattern, text++, case_insensitive));
  return false;
}

// Greedy (*) quantifier
bool match_star(const char *pattern, const char *regex, const char *text,
                bool case_insensitive, bool dot_all, bool multi_line) {
  return match_star_general(pattern, regex, text, case_insensitive, dot_all,
                            multi_line, false);
}

// Lazy (*?) quantifier
bool match_star_lazy(const char *pattern, const char *regex, const char *text,
                     bool case_insensitive, bool dot_all, bool multi_line) {
  return match_star_general(pattern, regex, text, case_insensitive, dot_all,
                            multi_line, true);
}

// Possessive (*+) quantifier (no backtracking)
bool match_star_possessive(const char *pattern, const char *regex,
                           const char *text, bool case_insensitive,
                           bool dot_all, bool multi_line) {
  while (*text != '\0' && match_pattern(pattern, text++, case_insensitive)) {
    // Match as many as possible, but no backtracking
  }
  return match_here(regex, text, case_insensitive, dot_all,
                    multi_line); // No backtracking
}

// Optimized lazy range quantifier (e.g., +?, {min,max}?)
bool match_lazy_range(const char *pattern, const char *regex, const char *text,
                      int min, int max, bool case_insensitive, bool dot_all,
                      bool multi_line) {
  int count = 0;
  while (count < max && *text != '\0' &&
         match_pattern(pattern, text, case_insensitive)) {
    text++;
    count++;
    if (count >= min &&
        match_here(regex, text, case_insensitive, dot_all, multi_line)) {
      return true; // Stop early for lazy match
    }
  }
  return false;
}

// Optimized greedy range quantifier (e.g., {min,max})
bool match_range(const char *pattern, const char *regex, const char *text,
                 int min, int max, bool case_insensitive, bool dot_all,
                 bool multi_line) {
  int count = 0;
  while (count < max && *text != '\0' &&
         match_pattern(pattern, text++, case_insensitive)) {
    count++;
  }
  return count >= min &&
         match_here(regex, text, case_insensitive, dot_all, multi_line);
}

// Error handling for invalid quantifier usage (min > max)
bool validate_range(int min, int max) {
  if (min > max || min < 0 || max < 0) {
    fprintf(stderr, "Error: Invalid quantifier range {%d,%d}\n", min, max);
    return false;
  }
  return true;
}
