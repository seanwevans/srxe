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
  if (argc < 2) {
    const char *regex = "a*b";
    const char *text = "aaab";  
  } else {
    const char *regex = argv[1];
    const char *text = argv[2];
  }

  _srxe(regex, text);
  
  return 0;
}
