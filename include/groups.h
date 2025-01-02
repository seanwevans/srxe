// groups.h

#ifndef SRX_GROUPS_H
#define SRX_GROUPS_H

// Handle alternation (|)
bool match_alternation(const char *regex, const char *text,
                       bool case_insensitive, bool dot_all, bool multi_line)

// Handle atomic groups (?>...)
bool match_atomic_group(const char *regex, const char *text,
                        bool case_insensitive, bool dot_all,
                        bool multi_line)

// Subroutine Calls ((?1), (?&name))
bool match_subroutine(const char *regex, const char *text,
                      bool case_insensitive, bool dot_all, bool multi_line,
                      int group_num)

// Handle conditional matching ((?(condition)yes|no))
bool match_conditional(const char *regex, const char *text,
                       bool condition_met, bool case_insensitive,
                       bool dot_all, bool multi_line)

// Handle anchors within groups (^, $, \A, \Z, \z)
bool match_anchor(const char *regex, const char *text, const char *start,
                  bool case_insensitive, bool dot_all, bool multi_line)

// Non-capturing group handling
bool match_non_capturing_group(const char *regex, const char *text,
                               bool case_insensitive, bool dot_all,
                               bool multi_line)

#endif // SRX_GROUPS_H