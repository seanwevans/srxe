// Sean's Regular eXpression Engine
//   version 0.1

// main.c

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "srxe.h"

void _srxe(const char* regex, const char* text) {
  FSM *fsm = compile(regex);
  match(fsm, text) ? 
    printf("Match found!\n") : 
    printf("No match.\n");
  free_fsm(fsm);
}


int main(int argc, char* argv[]) {
  const char *regex;
  const char *text;

  if (argc < 2) {
    regex = "a*b";
    text = "aaab";
  } else {
    regex = argv[1];
    text = argv[2];
  }

  _srxe(regex, text);
  
  return 0;
}
