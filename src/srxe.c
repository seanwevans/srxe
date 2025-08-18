// srxe.c

#include "srxe.h"
#include <stdlib.h>
#include <string.h>

State *create_state(StateType type, char match_char) {
  State *state = (State *)malloc(sizeof(State));
  state->type = type;
  state->match_char = match_char;
  state->out_count = 0;
  state->out_capacity = INITIAL_TRANSITIONS;
  state->out = (State **)malloc(INITIAL_TRANSITIONS * sizeof(State *));
  return state;
}

void add_transition(State *from, State *to) {
  if (from->out_count >= from->out_capacity) {
    from->out_capacity *= 2;
    from->out =
        (State **)realloc(from->out, from->out_capacity * sizeof(State *));
  }
  from->out[from->out_count++] = to;
}
static State *compile_sequence(const char *regex, State *start) {
  State *current = start;
  while (*regex != '\0' && *regex != '|') {
    switch (*regex) {
    case '.': {
      State *dot_state = create_state(DOT, '\0');
      add_transition(current, dot_state);
      current = dot_state;
      break;
    }

    case '*': {
      State *star_state = create_state(STAR, '\0');
      add_transition(current, star_state);
      current = star_state;
      break;
    }

    case '+': {
      State *plus_state = create_state(PLUS, '\0');
      add_transition(current, plus_state);
      current = plus_state;
      break;
    }

    case '?': {
      State *question_state = create_state(QUESTION, '\0');
      add_transition(current, question_state);
      current = question_state;
      break;
    }

    case '[': {
      int class_len;
      State *char_class_state = create_state(CHAR_CLASS, '\0');
      add_transition(current, char_class_state);
      current = char_class_state;
      regex += class_len;
      continue;
    }

    default: {
      State *char_state = create_state(CHAR, *regex);
      add_transition(current, char_state);
      current = char_state;
      break;
    }
    }
    regex++;
  }
  return current;
}

FSM *compile(const char *regex) {
  FSM *fsm = (FSM *)malloc(sizeof(FSM));
  State *start_state = create_state(BEGIN, '\0');
  fsm->start = start_state;
  State *accept_state = create_state(ACCEPT, '\0');

  const char *branch_pos = strchr(regex, '|');
  if (branch_pos) {
    size_t left_len = branch_pos - regex;
    char *left = (char *)malloc(left_len + 1);
    strncpy(left, regex, left_len);
    left[left_len] = '\0';
    char *right = strdup(branch_pos + 1);

    State *branch_state = create_state(BRANCH, '\0');
    add_transition(start_state, branch_state);

    State *left_end = compile_sequence(left, branch_state);
    State *right_end = compile_sequence(right, branch_state);

    add_transition(left_end, accept_state);
    add_transition(right_end, accept_state);

    free(left);
    free(right);
  } else {
    State *end = compile_sequence(regex, start_state);
    add_transition(end, accept_state);
  }

  return fsm;
}

bool match(FSM *fsm, const char *text) {
  State *current = fsm->start;

  while (current != NULL) {
    switch (current->type) {
    case CHAR:
      if (*text == current->match_char) {
        text++;
        current = current->out[0];
      } else {
        return false;
      }
      break;

    case DOT:
      if (*text != '\0') {
        text++;
        current = current->out[0];
      } else {
        return false;
      }
      break;

    case STAR:
      while (*text != '\0' && *text == current->match_char) {
        text++;
      }
      current = current->out[0];
      break;

    case BRANCH:
      if (current->out_count >= 2) {
        return match(current->out[0], text) || match(current->out[1], text);
      } else if (current->out_count == 1 && current->out[0] != NULL) {
        return match(current->out[0], text);
      } else {
        return false;
      }

    case ACCEPT:
      return true;

    default:
      return false;
    }
  }
  return false;
}

void free_fsm(FSM *fsm) {
  if (fsm == NULL)
    return;
  free_state(fsm->start);
  free(fsm);
}

void free_state(State *state) {
  if (state == NULL)
    return;
  for (int i = 0; i < state->out_count; i++) {
    free_state(state->out[i]);
  }
  free(state->out);
  free(state);
}
