// srxe.c

void add_transition(State *from, State *to) {
  if (from->out_count >= from->out_capacity) {
    from->out_capacity *= 2;
    from->out =
        (State **)realloc(from->out, from->out_capacity * sizeof(State *));
  }
  from->out[from->out_count++] = to;
}

bool char_in_class(char c, const char *regex, int *class_len) {
  const char *start = regex;
  bool negate = false;
  bool match = false;

  if (*regex == '^') {
    negate = true;
    regex++;
  }

  while (*regex != ']' && *regex != '\0') {
    if (*(regex + 1) == '-' && *(regex + 2) != ']') {
      if (c >= *regex && c <= *(regex + 2)) {
        match = true;
      }
      regex += 3;
    } else {
      if (c == *regex) {
        match = true;
      }
      regex++;
    }
  }

  *class_len = (regex - start) + 1;
  return negate ? !match : match;
}

bool match_unicode_property(char property, char c, bool negate) {
  bool result = false;
  switch (property) {
  case 'L':
    result = iswalpha(c);
    break; // Letters
  case 'N':
    result = iswdigit(c);
    break; // Numbers
  case 'P':
    result = ispunct(c);
    break; // Punctuation
  case 'S':
    result = isspace(c);
    break; // Whitespace
  }
  return negate ? !result : result;
}

bool match_unicode_casefold(const char *regex, const char *text, bool case_insensitive) {
  return case_insensitive ? tolower(*regex) == tolower(*text) : *regex == *text;
}

FSM *compile(const char *regex) {
  FSM *fsm = (FSM *)malloc(sizeof(FSM));
  State *start_state = create_state(BEGIN, '\0');
  fsm->start = start_state;

  State *current = start_state;
  while (*regex != '\0') {
    switch (*regex) {
    case '.':
      current->out[0] = create_state(DOT, '\0');
      current = current->out[0];
      break;

    case '*':
      current->out[0] = create_state(STAR, '\0');
      current = current->out[0];
      break;

    case '|':
      State *branch_state = create_state(BRANCH, '\0');
      add_transition(current, branch_state);
      current = branch_state;
      break;

    case '+':
      current->out[0] = create_state(PLUS, '\0');
      current = current->out[0];
      break;

    case '?':
      current->out[0] = create_state(QUESTION, '\0');
      current = current->out[0];
      break;

    case '[':
      // Handle character classes
      {
        int class_len;
        State *char_class_state = create_state(CHAR_CLASS, '\0');
        current->out[0] = char_class_state;
        current = char_class_state;
        regex += class_len;
      }
      break;

    default:
      // Default case handles specific character matching
      current->out[0] = create_state(CHAR, *regex);
      current = current->out[0];
      break;
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
