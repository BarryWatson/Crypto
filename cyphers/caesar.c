/*
 * caesar.c
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
 * A simple implementation of the caesar shift cypher.
 * Try
 *    echo  "Hello World 123" | ./caesar -e -s3
 *    echo  "Hello World 123" | ./caesar -e -s3  | ./caesar -d -s3
 *
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFLEN 128 

void cs_usage (void);
void cs_process(unsigned int shift, char *buffer, size_t len, char encrypt_flag);

int main(int argc, char ** argv)
{
  char buff[BUFFLEN];
  char encrypt = 0;
  char decrypt = 0;
  int shift = -1;
  char usage = 0;
  char ch;
  
  // Get options
  while ((ch = getopt(argc, argv, "eds:")) != -1) {
    switch (ch) {
    case 'e':
      encrypt = 1;
      break;
      
    case 'd':
      decrypt = 1;
      break;
      
    case 's':
      shift = atoi(optarg);
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

  /*
   * The shift is our key.
   * Our keyspace is only 25 elements.
   */
  if (shift >= 26 || shift <= 0) {
    usage = 1;
  }
  
  if (usage) {
    cs_usage();
    exit(-1);
  }
  
  /* Perform instructed task */
  while (fgets(buff, BUFFLEN, stdin)) {
    if (encrypt) {
      cs_process(shift, buff, strlen(buff) - 1, 1); /* -1 to ignore terminal '\n' */
    }
    else {
      cs_process(shift, buff, strlen(buff) - 1, 0); /* -1 to ignore terminal '\n' */
    }
  }

  return EXIT_SUCCESS;
}

/*
 * We can't use the '%' operator for modulo as it is
 * actaully a remainder operator which will work if the
 * left hand argument is positive, but not if it is negative.
 */
int modulo(int a, int b)
{
  int m;

  m = a % b;
  if (m < 0) {
    m = m + b;
  }
  
  return m;
}

void cs_process(unsigned int shift, char *buffer, size_t len, char encrypt_flag)
{
  size_t i;
  char ch;
  char base;
  
  for (i=0; i<len; i++) {
    ch = buffer[i];
    if (isalpha(ch)) {
      if (isupper(ch)) {
	base = 'A';
      } else {
	base = 'a';
      }
      ch = ch - base;
      if (encrypt_flag) {
	ch = modulo(ch + shift, 26);
      } else {
	ch = modulo(ch - shift, 26);
      }
      ch = ch + base;
    }
    putchar(ch);
  }
  putchar('\n');
}

void cs_usage()
{
  puts("caesar [-d|-e] -s SHIFT");
  puts("\t-d decrypt stdin to stdout.");
  puts("\t-e encrypt stdin to stdout.");
  puts("\t-s SHIFT the shift tp apply.");
}
