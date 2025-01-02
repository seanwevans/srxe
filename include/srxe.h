// srxe.h

#ifndef SRX_SRXE_H
#define SRX_SRXE_H

#define INITIAL_TRANSITIONS 4

void add_transition(State *from, State *to);
bool char_in_class(char c, const char *regex, int *class_len);
bool match_unicode_property(char property, char c, bool negate);
bool match_unicode_casefold(const char *regex, const char *text, bool case_insensitive);
FSM *compile(const char *regex);
bool match(FSM *fsm, const char *text);
void free_fsm(FSM *fsm);
void free_state(State *state);

typedef enum {
  CHAR,                // Match a specific character
  CHAR_CLASS,          // Match a character class
  STAR,                // Star quantifier (greedy)
  PLUS,                // Plus quantifier
  QUESTION,            // Question mark quantifier (?)
  DOT,                 // Dot (.) matching any character
  BEGIN,               // Beginning of string (^)
  END,                 // End of string ($)
  BRANCH,              // Alternation (|)
  ACCEPT,              // Accepting state (end of the pattern)
  ASSERT_LOOKAHEAD,    // Lookahead assertion (?= or (?!)
  ASSERT_LOOKBEHIND,   // Lookbehind assertion (?<= or ?<!)
  ASSERT_WORD_BOUNDARY // Word boundary \b or \B
} StateType;

typedef struct State {
  StateType type;
  char match_char;    // Character to match in CHAR or CHAR_CLASS
  struct State **out; // Dynamically allocated array of outgoing transitions
  int out_count;      // Current number of transitions
  int out_capacity;   // Current capacity of the outgoing transitions
} State;

typedef struct FSM {
  State *start;
} FSM;

State *create_state(StateType type, char match_char) {
  State *state = (State *)malloc(sizeof(State));
  state->type = type;
  state->match_char = match_char;
  state->out_count = 0;
  state->out_capacity = INITIAL_TRANSITIONS;
  state->out = (State **)malloc(INITIAL_TRANSITIONS * sizeof(State *));
  return state;
}

#endif // SRX_SRXE_H
