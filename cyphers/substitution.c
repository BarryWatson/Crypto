/*
 * substitution.c
 *
 * Copyright (c) 2025 Barry Watson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
 *
 *
 * A simple implementation of the substitution cypher.
 * Try
 *    echo  "Hello World 123" | ./substitution -e
 *    echo  "Hello World 123" | ./substitution -e  | ./substitution -d
 *    echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz" | ./substitution -e | ./substitution -d
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void s_usage (void);
char s_encrypt(int ch);
char s_decrypt(int ch);

/*
 * The substitution table is our cypher's key.
 * Here, we only substitute alphabetical characters.
 * The keyspace is thus 26! = 2^88 [log(26!)/log(2) = 88.38]. 
 */
char substitution_table[] = {
/* A    B    C    D    E    F    G    H    I   */
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O',
/* J    K    L    M    N    O    P    Q    R   */
  'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
/* S    T    U    V    W    X    Y    Z        */
  'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
};

int main(int argc, char ** argv)
{
  char encrypt = 0;
  char decrypt = 0;
  char usage = 0;
  int ch;
  
  // Get options
  while ((ch = getopt(argc, argv, "ed")) != -1) {
    switch (ch) {
    case 'e':
      encrypt = 1;
      break;
      
    case 'd':
      decrypt = 1;
      break;
            
    case '?':
      usage = 1;
      break;
    }
  }
  
  /* Validate input */
  if ((encrypt ^ decrypt) == 0) { /* One and only one must be set */
    usage = 1;
  }
  
  if (usage) {
    s_usage();
    exit(-1);
  }
  
  /* Perform instructed task */
  if (encrypt) {
    while ((ch = getchar()) != EOF) {
      putchar(s_encrypt(ch));
    }
  } else {
    while ((ch = getchar()) != EOF) {
      putchar(s_decrypt(ch));
    }
  }

  return EXIT_SUCCESS;
}


char s_encrypt(int plaintext)
{
  char cyphertext;
  
  if (isalpha(plaintext)) {
    if (isupper(plaintext)) {
	cyphertext = substitution_table[plaintext - 'A'];
      } else {
	cyphertext = tolower(substitution_table[plaintext - 'a']);
      }     
      return cyphertext;
  }  else {
    /* Only encrypt alphabetical characters */
    return plaintext;
  }
}

char s_decrypt(int cyphertext)
{
  int index;
  char wanted;
  char upper;

  if (isalpha(cyphertext)) {
    if (isupper(cyphertext)) {
	upper = 1;
	wanted = cyphertext;
      } else {
	upper = 0;
	wanted = toupper(cyphertext);
      }
    
    for (index = 0; index < 26; index++) {
      if (wanted == substitution_table[index]) {
	return upper ? 'A' + index : 'a' + index;
      }
    }
    fprintf(stderr, "\n\n\nBadly formed substitution table\n\n");
    exit(EXIT_FAILURE);
    return cyphertext; /* Keep the compiler happy */    
  } else {
    return cyphertext;
  }
}

void s_usage()
{
  puts("substitution [-d|-e]");
  puts("\t-d decrypt stdin to stdout.");
  puts("\t-e encrypt stdin to stdout.");
}
