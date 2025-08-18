// srxe.c

#include "srxe.h"

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



FSM *compile(const char *regex) {
  FSM *fsm = (FSM *)malloc(sizeof(FSM));
  State *start_state = create_state(BEGIN, '\0');
  fsm->start = start_state;

  State *current = start_state;
  while (*regex != '\0') {
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

    case '|': {
      State *branch_state = create_state(BRANCH, '\0');
      add_transition(current, branch_state);
      current = branch_state;
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

    case '[':
      // Handle character classes
      {
        int class_len;
        State *char_class_state = create_state(CHAR_CLASS, '\0');
        add_transition(current, char_class_state);
        current = char_class_state;
        regex += class_len;
      }
      break;

    default: {
      // Default case handles specific character matching
      State *char_state = create_state(CHAR, *regex);
      add_transition(current, char_state);
      current = char_state;
      break;
    }
    }
    regex++;
  }

  // Add an accept state at the end of the FSM
  State *accept_state = create_state(ACCEPT, '\0');
  add_transition(current, accept_state);
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
      return match(current->out[0], text) || match(current->out[1], text);

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
