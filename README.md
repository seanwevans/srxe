# Sean's Regular eXpression Engine (SRXE)

## Overview

Sean's Regular eXpression Engine (SRXE) is a lightweight and customizable regex engine implemented in C. The project aims to provide a foundation for building and executing regular expressions without relying on external libraries.

This project consists of the following key files:

- `main.c`: Contains the entry point for SRXE, handling argument parsing and initiating the matching process.
- `engine.c`: Implements the core matching logic, including support for lookaheads, lookbehinds, atomic groups, and more.
- `srxe.c`: Responsible for constructing the finite state machine (FSM) used for regex matching, managing transitions, and handling character classes.
- `character_classes.c`: Manages character classes, shorthand classes, and character ranges.
- `escapes.c`: Handles escape sequences (e.g., `\d`, `\w`, `\s`) and POSIX class expressions.
- `groups.c`: Implements alternations, atomic groups, and conditional subroutine calls.
- `lookaround.c`: Supports lookahead and lookbehind assertions.
- `posix.c`: Manages POSIX character classes such as `[:digit:]` and `[:alpha:]`.
- `quantifiers.c`: Implements greedy, lazy, and possessive quantifiers.
- `unicode.c`: Handles Unicode properties, scripts, and case folding.
- `utils.c`: Utility functions for parsing and handling character classes, hex, and Unicode conversion.
- `Makefile`: Contains build instructions for compiling the project.

### Header Files:
- `engine.h`: Declares the main regex matching functions and defines constants for group capturing.
- `srxe.h`: Defines core structures for FSM states and transitions, and provides functions for FSM compilation and deallocation.
- `character_classes.h`: Declares functions for handling character classes, shorthand classes, and class operations.
- `escapes.h`: Declares functions for processing escape sequences.
- `groups.h`: Provides declarations for alternations, atomic groups, and group-based operations.
- `lookaround.h`: Declares functions for lookahead and lookbehind assertions.
- `posix.h`: Declares functions for POSIX character class matching.
- `quantifiers.h`: Declares quantifier handling functions, including greedy, lazy, and possessive operations.
- `unicode.h`: Declares Unicode matching functions and case folding support.
- `utils.h`: Provides helper functions for character conversion, parsing, and utility operations.

## Features

- **Support for Basic and Advanced Regex Constructs**:
  - Character classes (e.g., `[a-z]`)
  - Quantifiers (`*`, `+`, `?`, `{min,max}`)
  - Lookaheads and lookbehinds
  - Non-capturing and atomic groups
  - Unicode property matching (`\p{}`)
  - POSIX character class support (`[:digit:]`, `[:alpha:]`)
  - Escape sequences (e.g., `\d`, `\w`, `\s`)
- **Finite State Machine (FSM) Compilation**: The regex is compiled into an FSM to enable efficient matching.
- **Custom Matching Logic**: Case-insensitive matching, dot-all mode, and multi-line support.

## Project Structure

```
.
├── main.c       # Entry point for the regex engine
├── engine.c     # Core matching functions
├── srxe.c       # FSM construction and matching
├── character_classes.c  # Character class handling
├── escapes.c    # Escape sequences and POSIX classes
├── groups.c     # Alternations and groups
├── lookaround.c # Lookaheads and lookbehinds
├── posix.c      # POSIX class handling
├── quantifiers.c # Quantifier handling
├── unicode.c    # Unicode property matching
├── utils.c      # Utility functions
├── engine.h     # Header file for engine.c
├── srxe.h       # FSM definitions and core structures
├── character_classes.h  # Header for character classes
├── escapes.h    # Header for escape sequences
├── groups.h     # Header for group operations
├── lookaround.h # Header for lookaround assertions
├── posix.h      # Header for POSIX classes
├── quantifiers.h # Header for quantifiers
├── unicode.h    # Header for Unicode operations
└── utils.h      # Header for utility functions
```

## How It Works

### Main Flow

1. **Regex Compilation**: The regex is passed to `srxe.c`, where it is compiled into an FSM.
2. **Matching**: The FSM is traversed to determine if the input text matches the regex pattern.
3. **Result**: A match result is printed to the console.

### Example

```
$ ./srxe a*b aaab
Match found!
```

If no match is found, the engine outputs:

```
No match.
```

## Compilation and Execution

To compile the project, use the following command:

```bash
make
```

To run the compiled binary:

```bash
./srxe [regex] [text]
```

If no arguments are provided, the program defaults to matching `a*b` against `aaab`.

## Implementation Details

- **FSM Construction**: `srxe.c` constructs an FSM from the regex by parsing each character and handling control structures.
- **Character Classes**: `character_classes.c` processes character ranges, shorthand classes, and inline case folding.
- **Escape Sequences**: `escapes.c` interprets escape sequences and POSIX classes.
- **Quantifiers**: `quantifiers.c` implements greedy, lazy, and possessive quantifiers.
- **Groups and Alternations**: `groups.c` manages alternations, atomic groups, and conditional subroutine calls.
- **Lookarounds**: `lookaround.c` handles lookahead and lookbehind assertions.
- **POSIX Classes**: `posix.c` supports POSIX character classes.
- **Unicode Matching**: `unicode.c` manages Unicode properties, scripts, and case folding.
- **Utility Functions**: `utils.c` provides helpers for parsing and converting escape sequences.

## Future Improvements

- Add support for capturing groups and backreferences.
- Improve performance through FSM optimization.
- Implement lazy and possessive quantifiers.

## Author

Sean - version 0.1

