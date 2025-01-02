// engine.c

#include "engine.h"

// Match the text with the given regex
bool match_here(const char *regex, const char *text, bool case_insensitive,
                bool dot_all, bool multi_line) {
  if (*regex == '\0') {
    return true;
  }

  // Alternation (|)
  if (*regex == '|') {
    return match_alternation(regex + 1, text, case_insensitive, dot_all,
                             multi_line);
  }

  // Lookaheads
  if (*regex == '(' && *(regex + 1) == '?' &&
      (*(regex + 2) == '=' || *(regex + 2) == '!')) {
    bool positive = (*(regex + 2) == '=');
    return match_lookahead(regex + 3, text, positive, case_insensitive, dot_all,
                           multi_line);
  }

  // Lookbehinds
  if (*regex == '(' && *(regex + 1) == '?' &&
      (*(regex + 2) == '<' && (*(regex + 3) == '=' || *(regex + 3) == '!'))) {
    bool positive = (*(regex + 3) == '=');
    return match_lookbehind(regex + 4, text, positive, case_insensitive,
                            dot_all, multi_line, 3); // Example length
  }

  // Lookahead assertions
  if (*regex == '(' && *(regex + 1) == '?' &&
      (*(regex + 2) == '=' || *(regex + 2) == '!')) {
    bool positive = (*(regex + 2) == '=');
    return match_lookahead(regex + 3, text, positive, case_insensitive, dot_all,
                           multi_line);
  }

  // Lookbehind assertions
  if (*regex == '(' && *(regex + 1) == '?' &&
      (*(regex + 2) == '<' && (*(regex + 3) == '=' || *(regex + 3) == '!'))) {
    bool positive = (*(regex + 3) == '=');
    return match_lookbehind(regex + 4, text, positive, case_insensitive,
                            dot_all, multi_line, 3); // Fixed-length example
  }

  // Atomic groups (?>...)
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == '>') {
    return match_atomic_group(regex + 3, text, case_insensitive, dot_all,
                              multi_line);
  }

  // Non-capturing groups (?:...)
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == ':') {
    return match_non_capturing_group(regex + 3, text, case_insensitive, dot_all,
                                     multi_line);
  }

  // Handle intersection/subtraction in character classes
  if (*regex == '[' && *(regex + 1) != ']') {
    return match_intersection_class(regex + 1, text, case_insensitive) &&
           match_here(regex + 1, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  // Handle shorthand classes (\h, \v)
  if (*regex == '\\' && (*(regex + 1) == 'h' || *(regex + 1) == 'v')) {
    return match_shorthand_class(*(regex + 1), *text) &&
           match_here(regex + 2, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  // Handle Unicode case folding
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == 'i') {
    return match_unicode_casefold(regex + 4, text, true) &&
           match_here(regex + 4, text + 1, true, dot_all, multi_line);
  }

  // Handle subroutine calls
  if (*regex == '(' && *(regex + 1) == '?') {
    if (*(regex + 2) >= '1' && *(regex + 2) <= '9') {
      int group_num = *(regex + 2) - '0';
      return match_subroutine(regex + 3, text, case_insensitive, dot_all,
                              multi_line, group_num);
    }
  }

  // Handle conditional matching
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == '(') {
    bool condition_met = (captured_groups[1][0] != '\0');
    return match_conditional(regex + 5, text, condition_met, case_insensitive,
                             dot_all, multi_line);
  }

  // Handle anchors within groups
  if (*regex == '^' || *regex == '$') {
    return match_anchor(regex, text, text, case_insensitive, dot_all,
                        multi_line);
  }

  // Handle multi-character character classes
  if (*regex == '[' && *(regex + 1) != ']') {
    return match_multichar_class(regex + 1, text, case_insensitive) &&
           match_here(regex + 1, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  // Handle Unicode script support
  if (*regex == '\\' && (*(regex + 1) == 'p' || *(regex + 1) == 'P')) {
    bool negate = (*(regex + 1) == 'P');
    return match_unicode_script(*(regex + 3), *text, negate) &&
           match_here(regex + 5, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  // Anchors (\A, \Z, \z)
  if (*regex == '\\' &&
      (*(regex + 1) == 'A' || *(regex + 1) == 'Z' || *(regex + 1) == 'z')) {
    if (*(regex + 1) == 'A' && text == text) {
      return match_here(regex + 2, text, case_insensitive, dot_all, multi_line);
    } else if (*(regex + 1) == 'Z' && *text == '\0') {
      return match_here(regex + 2, text, case_insensitive, dot_all, multi_line);
    } else if (*(regex + 1) == 'z' && *text == '\0') {
      return true;
    }
  }

  // Inline comments (?#...)
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == '#') {
    while (*regex != ')' && *regex != '\0') {
      regex++;
    }
    return match_here(regex + 1, text, case_insensitive, dot_all, multi_line);
  }

  // Fixed-length lookbehind assertions ((?<=...) and (?<!...))
  if (*regex == '(' && *(regex + 1) == '?' && (*(regex + 2) == '<')) {
    bool positive = (*(regex + 3) == '=');
    return match_lookbehind(regex + 4, text, positive, case_insensitive) &&
           match_here(regex + 4, text, case_insensitive, dot_all, multi_line);
  }

  // Character class matching
  if (*regex == '[') {
    return match_inline_casefold(regex + 1, text, case_insensitive) &&
           match_here(regex + 1, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  // Handle non-greedy and possessive quantifiers
  if (*(regex + 1) == '?' && *regex == '*') {
    return match_star_lazy(*(regex - 1), regex + 2, text, case_insensitive,
                           dot_all, multi_line);
  } else if (*(regex + 1) == '+' && *regex == '*') {
    return match_star_possessive(*(regex - 1), regex + 2, text,
                                 case_insensitive, dot_all, multi_line);
  }

  // Handle non-greedy {min,} quantifiers
  if (*regex == '{') {
    int min;
    parse_braces(regex, &min);
    return match_range_infinite(*(regex - 1), regex + 1, text, min,
                                case_insensitive, dot_all, multi_line);
  }

  // Handle escape sequences
  if (*regex == '\\' && (*(regex + 1) == 'x' || *(regex + 1) == 'u')) {
    // Handle \xHH or \uHHHH escape sequences
    int num_chars = (*(regex + 1) == 'x') ? 2 : 4;
    if (match_escape(*(regex + 1), regex + 2, text, case_insensitive)) {
      return match_here(regex + num_chars + 2, text + 1, case_insensitive,
                        dot_all, multi_line);
    }

    return false;
  }

  // Atomic group handling (?>...)
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == '>') {
    // Non-backtracking atomic group: skip over the atomic part
    const char *end = strstr(regex, ")");
    if (!end)
      return false; // No closing parenthesis, invalid
    return match_here(end + 1, text, case_insensitive, dot_all, multi_line);
  }

  // Non-capturing group handling (?:...)
  if (*regex == '(' && *(regex + 1) == '?' && *(regex + 2) == ':') {
    // Non-capturing group, proceed inside the parentheses
    const char *group_end = strstr(regex, ")");
    if (!group_end)
      return false; // No closing parenthesis, invalid
    return match_here(regex + 3, text, case_insensitive, dot_all, multi_line) &&
           match_here(group_end + 1, text, case_insensitive, dot_all,
                      multi_line);
  }

  // Lookahead assertions (?=...) and (?!...)
  if (*regex == '(' && *(regex + 1) == '?') {
    if (*(regex + 2) == '=') {
      // Positive lookahead (?=...)
      const char *lookahead_end = strstr(regex, ")");
      if (!lookahead_end)
        return false;
      const char *lookahead_pattern = regex + 3;
      if (match_here(lookahead_pattern, text, case_insensitive, dot_all,
                     multi_line)) {
        return match_here(lookahead_end + 1, text, case_insensitive, dot_all,
                          multi_line);
      } else {
        return false;
      }
    } else if (*(regex + 2) == '!') {
      // Negative lookahead (?!...)
      const char *lookahead_end = strstr(regex, ")");
      if (!lookahead_end)
        return false;
      const char *lookahead_pattern = regex + 3;
      if (!match_here(lookahead_pattern, text, case_insensitive, dot_all,
                      multi_line)) {
        return match_here(lookahead_end + 1, text, case_insensitive, dot_all,
                          multi_line);
      } else {
        return false;
      }
    }
  }

  // Handle {min,max} quantifiers
  if (*regex == '{') {
    int min, max;
    parse_braces(regex, &min, &max);
    return match_range(*(regex - 1), regex + 1, text, min, max,
                       case_insensitive, dot_all, multi_line);
  }

  // Handle * quantifier
  if (*(regex + 1) == '*') {
    return match_star(*regex, regex + 2, text, case_insensitive, dot_all,
                      multi_line);
  }

  // Handle + quantifier
  if (*(regex + 1) == '+') {
    return (*text != '\0' && char_equals(*text, *regex, case_insensitive)) &&
           match_star(*regex, regex + 2, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  // Handle ? quantifier (0 or 1)
  if (*(regex + 1) == '?') {
    return match_here(regex + 2, text, case_insensitive, dot_all, multi_line) ||
           (*text != '\0' && char_equals(*text, *regex, case_insensitive) &&
            match_here(regex + 2, text + 1, case_insensitive, dot_all,
                       multi_line));
  }

  // Handle multi-line mode `^` (start of line)
  if (*regex == '^') {
    if (multi_line) {
      return (*text == '\0' || text == text || *(text - 1) == '\n') &&
             match_here(regex + 1, text, case_insensitive, dot_all, multi_line);
    } else {
      return text == text &&
             match_here(regex + 1, text, case_insensitive, dot_all, multi_line);
    }
  }

  // Handle multi-line mode `$` (end of line)
  if (*regex == '$' && *(regex + 1) == '\0') {
    if (multi_line) {
      return *text == '\0' || *text == '\n';
    } else {
      return *text == '\0';
    }
  }

  if (*regex == '[') {
    // Character class matching (e.g., [a-z])
    int class_len = 0;
    if (*text != '\0') {
      if (char_equals(*text, regex[1], case_insensitive)) {
        return match_here(regex + class_len + 1, text + 1, case_insensitive,
                          dot_all, multi_line); // Skip the character class
      }
    }
    return false;
  }

  // Handle \A and \Z anchors
  if (*regex == '\\' && (*(regex + 1) == 'A')) {
    if (text == original_text) { // Only match at the beginning of the string
      return match_here(regex + 2, text, case_insensitive, dot_all, multi_line);
    } else {
      return false;
    }
  }

  if (*regex == '\\') {
    // Handle escape sequences like \s, \d, \w, \D, \S, \W, \b, \B
    if (match_escape(*(regex + 1), *text, text, case_insensitive)) {
      return match_here(regex + 2, text + 1, case_insensitive, dot_all,
                        multi_line);
    } else {
      return false;
    }
  }

  // Handle dot-all mode
  if (*regex == '.' && (dot_all || *text != '\n')) {
    return match_here(regex + 1, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  if (*text != '\0' && char_equals(*regex, *text, case_insensitive)) {
    return match_here(regex + 1, text + 1, case_insensitive, dot_all,
                      multi_line);
  }

  return false;
}

// Entry point for regex matching
bool match(const char *regex, const char *text) {
  bool case_insensitive = false;
  bool dot_all = false;
  bool multi_line = false;

  // Process flags at the beginning of the regex
  while (*regex == '(' && *(regex + 1) == '?' && strchr("ism", *(regex + 2))) {
    if (*(regex + 2) == 'i') {
      case_insensitive = true;
    } else if (*(regex + 2) == 's') {
      dot_all = true;
    } else if (*(regex + 2) == 'm') {
      multi_line = true;
    }
    regex += 4; // Move past the flag
  }

  if (*regex == '^') {
    return match_here(regex + 1, text, case_insensitive, dot_all, multi_line);
  }

  do {
    if (match_here(regex, text, case_insensitive, dot_all, multi_line)) {
      return true;
    }
  } while (*text++ != '\0');

  return false;
}
