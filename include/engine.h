// engine.h

#ifndef SRX_ENGINE_H
#define SRX_ENGINE_H

#define MAX_GROUPS 256

char captured_groups[MAX_GROUPS][256];
int group_lengths[MAX_GROUPS];

bool match_here(const char *regex, const char *text, bool case_insensitive,
                bool dot_all, bool multi_line);

bool match(const char *regex, const char *text);

#endif // SRX_ENGINE_H
