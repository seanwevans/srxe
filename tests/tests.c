
// test.c

#include <stdio.h>
#include <stdbool.h>

#include "engine.h"

// Regular Expressions for testing
const char *rxs[] = {
    "a*b",
     "[a-z]*",
     "\\s*hello\\s*",
     "a{2,4}",
     "(?i)he*llo",
     "\\d+",
     "[a-z]+",
     "he|hi",
     "(?s)hello.world",
     "(?m)^foo$",
     "\\bword\\b",
     "\\D+",
     "(?=hello)hello",
     "(?>abc)def",
     "abc(?:def)ghi",
     "abc(?:xyz|def)ghi",
     "abc\\x41def",
     "hello\\u0065world",
     "\\p{L}+",
     "a{2,}",
     "\\Qspecial_chars?\\E",
     "(?i)[a-z]",
     "\\bhello\\b",
     "[[:alpha:]]",
     "a",
     "[a-z&&[^aeiou]]",
     "[[:alpha:]]",
     "[[:digit:]]",
     "[[:space:]]",
     "[[:xdigit:]]",
     "\\p{L}+",
     "a{2,}",
     "\\Qspecial_chars?\\E",
     "(?i)[a-z]",
     "\\bhello\\b",
     "[a-z&&[^aeiou]]",
     "a*?",
     "a*+",
     "a{2,}",
     "(cat|dog)",
     "[a-zA-Z]",
     "\\bhello\\b",
     "(?>a|ab)(c",
     "(?#this is a comment)hello",
     "(?<=abc)d",
     "(?<word>\\w+)",
     "(cat|dog)",
     "[a-zA-Z]",
     "\\bhello\\b",
     "(?>a|ab)c",
     "(?<=abc)d",
     "(?<word>\\w+)"
};

// Texts to test the regular expressions against
const char *texts[] = {
    "aaab",
    "hello",
    "   hello   ",
    "aaab",
    "HELLO",
    "12345",
    "abc",
    "hi",
    "hello\nworld",
    "bar\nfoo\nbaz",
    "This is a word.",
    "abc123",
    "hello world",
    "abcdef",
    "abcdefghi",
    "abcxyzghi",
    "abcAdef",
    "helloe world",
    "abcXYZ",
    "aaa",
    "special_chars?",
    "A",
    "hello world",
    "a",
    "A",
    "9",
    " ",
    "F",
    "abcXYZ",
    "aaa",
    "special_chars?",
    "A",
    "hello world",
    "b",
    "aaaa",
    "aaaab",
    "aaaaa",
    "dog",
    "A",
    "hello world",
    "ac",
    "hello",
    "abcd",
    "hello",
    "dog",
    "A",
    "hello world",
    "ac",
    "abcd",
    "hello",
};

// Helper function to run a single test case
void run_test(const char *test_name, const char *regex, const char *text,
              bool expected) {
  bool result = match(regex, text);
  if (result == expected) {
    printf("[PASS] %s\n", test_name);
  } else {
    printf("[FAIL] %s - Regex: '%s', Text: '%s', Expected: %s, Got: %s\n",
           test_name, regex, text, expected ? "true" : "false",
           result ? "true" : "false");
  }
}

// Unit Test Suite for Basic Features
void test_basic_features() {
  printf("Running tests for Basic Features...\n");

  // Literal Matching
  run_test("Literal Match", "hello", "hello", true);
  run_test("Literal Mismatch", "hello", "hell", false);

  // Anchors
  run_test("Anchor Start Match", "^hello", "hello world", true);
  run_test("Anchor Start Mismatch", "^hello", "world hello", false);
  run_test("Anchor End Match", "world$", "hello world", true);
  run_test("Anchor End Mismatch", "world$", "world hello", false);

  // Case Insensitive Matching
  run_test("Case Insensitive Match", "(?i)hello", "HELLO", true);
  run_test("Case Sensitive Mismatch", "hello", "HELLO", false);
}

// Unit Test Suite for Escape Sequences
void test_escape_sequences() {
  printf("Running tests for Escape Sequences...\n");

  // Word Boundaries
  run_test("Word Boundary Match", "\\bhello\\b", "hello world", true);
  run_test("Word Boundary Mismatch", "\\bhello\\b", "ahello world", false);

  // Whitespace
  run_test("Whitespace Match", "\\s", " ", true);
  run_test("Non-Whitespace Mismatch", "\\s", "a", false);

  // Digits
  run_test("Digit Match", "\\d", "5", true);
  run_test("Non-Digit Mismatch", "\\d", "a", false);

  // Word Characters
  run_test("Word Character Match", "\\w", "A", true);
  run_test("Non-Word Character Mismatch", "\\w", "$", false);

  // Literal Backslash
  run_test("Literal Backslash Match", "\\\\", "\\", true);
}

// Unit Test Suite for POSIX Classes
void test_posix_classes() {
  printf("Running tests for POSIX Classes...\n");

  // POSIX Bracket Expressions
  run_test("POSIX Alpha Match", "[[:alpha:]]", "A", true);
  run_test("POSIX Alpha Mismatch", "[[:alpha:]]", "1", false);

  run_test("POSIX Digit Match", "[[:digit:]]", "1", true);
  run_test("POSIX Digit Mismatch", "[[:digit:]]", "A", false);

  run_test("POSIX Alnum Match", "[[:alnum:]]", "A", true);
  run_test("POSIX Alnum Match Digit", "[[:alnum:]]", "1", true);
  run_test("POSIX Alnum Mismatch", "[[:alnum:]]", "$", false);

  run_test("POSIX Space Match", "[[:space:]]", " ", true);
  run_test("POSIX Space Mismatch", "[[:space:]]", "A", false);

  run_test("POSIX Xdigit Match", "[[:xdigit:]]", "F", true);
  run_test("POSIX Xdigit Mismatch", "[[:xdigit:]]", "G", false);
}

// Unit Test Suite for Quantifiers
void test_quantifiers() {
  printf("Running tests for Quantifiers...\n");

  // Greedy Quantifiers
  run_test("Greedy Star Match", "a*", "aaa", true);
  run_test("Greedy Star Match Empty", "a*", "", true);
  run_test("Greedy Plus Match", "a+", "aaa", true);
  run_test("Greedy Plus Mismatch", "a+", "", false);
  run_test("Greedy Question Mark Match", "a?", "a", true);
  run_test("Greedy Question Mark Match Empty", "a?", "", true);

  // Non-Greedy Quantifiers
  run_test("Non-Greedy Star Match", "a*?", "aaaa", true);
  run_test("Non-Greedy Star Match Empty", "a*?", "", true);
  run_test("Non-Greedy Plus Match", "a+?", "aaa", true);
  run_test("Non-Greedy Plus Mismatch", "a+?", "", false);

  // Possessive Quantifiers
  run_test("Possessive Star Match", "a*+", "aaaa", true);
  run_test("Possessive Star Mismatch", "a*+", "aaaab", false);
  run_test("Possessive Plus Match", "a++", "aaaa", true);
  run_test("Possessive Plus Mismatch", "a++", "aaaab", false);
}

// Unit Test Suite for Unicode Properties
void test_unicode_properties() {
  printf("Running tests for Unicode Properties...\n");

  run_test("Unicode Letter Match", "\\p{L}", "A", true);
  run_test("Unicode Letter Mismatch", "\\p{L}", "1", false);

  run_test("Unicode Digit Match", "\\p{N}", "1", true);
  run_test("Unicode Digit Mismatch", "\\p{N}", "A", false);

  run_test("Unicode Punctuation Match", "\\p{P}", "!", true);
  run_test("Unicode Punctuation Mismatch", "\\p{P}", "A", false);
}

// Unit Test Suite for Set Operations in Character Classes
void test_set_operations() {
  printf("Running tests for Set Operations in Character Classes...\n");

  run_test("Intersection Match", "[a-z&&[^aeiou]]", "b",
           true); // Should match consonants
  run_test("Intersection Mismatch", "[a-z&&[^aeiou]]", "a", false);

  run_test("Negation Match", "[^a-z]", "1",
           true); // Should match non-lowercase letters
  run_test("Negation Mismatch", "[^a-z]", "a", false);
}

int main(void) {
  size_t num_rxs = sizeof(rxs) / sizeof(rxs[0]);
  size_t num_texts = sizeof(texts) / sizeof(texts[0]);
  size_t count = num_rxs < num_texts ? num_rxs : num_texts;

  printf("Running pair tests...\n");
  for (size_t i = 0; i < count; ++i) {
    char name[32];
    snprintf(name, sizeof(name), "Pair %zu", i + 1);
    run_test(name, rxs[i], texts[i], true);
  }

  printf("\nRunning unit tests...\n");
  test_basic_features();
  test_escape_sequences();
  test_posix_classes();
  test_quantifiers();
  test_unicode_properties();
  test_set_operations();

  return 0;
}
