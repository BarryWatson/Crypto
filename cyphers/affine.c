/*
 * affine.c
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
 * A simple implementation of an affine cypher.
 * Try
 *    echo "Hello World" | ./affine -m 19 -a 3 -e 
 *    echo "Hello World" | ./affine -m 19 -a 3 -e | ./affine -m 19 -a 3 -d
 *
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void a_usage (void);
int gcd(int, int);
int calculate_inverse(int);
void a_encrypt(int, int);
void a_decrypt(int, int);
void error_msg(char *);
void abort_msg(char *);

int main(int argc, char ** argv)
{
  char encrypt = 0;
  char decrypt = 0;
  int mul = 0;
  char mul_flag = 0;
  int add = 0;
  char add_flag = 0;
  char usage = 0;
  char ch;
  
  // Get options
  while ((ch = getopt(argc, argv, "eda:m:")) != -1) {
    switch (ch) {
    case 'e':
      encrypt = 1;
      break;
      
    case 'd':
      decrypt = 1;
      break;
      
    case 'a':
      add = atoi(optarg);
      add_flag = 1;
      break;
      
    case 'm':
      mul = atoi(optarg);
      mul_flag = 1;
      break;
      
    case '?':
      usage = 1;
      break;
    }
  }
  
  /* Validate input */
  if ((encrypt ^ decrypt) == 0) { /* One and only one must be set */
    error_msg("Missing decrypt or encrypt option."); 
    usage = 1;
  }

  /*
   * Our keyspace is (m, a) where 2 <= m <=255, 0 <= a <=255, and gcd(mn, 255) = 1.
   */
  if (!mul_flag) {
    error_msg("-m missing.");
    usage = 1;
  } else if (!(mul >= 2 || mul <= 255)) {
    error_msg("-m parameter out of range.");
    usage = 1;
  } else if (gcd(mul, 255) != 1) {
    error_msg("-a parameter fails gcd test.");
    usage = 1;
  }

  if (!add_flag) {
    error_msg("-a missing.");
    usage = 1;
  } else if (!(add >= 0 || add <= 255)) {
    error_msg("-a parameter out of range.");
    usage = 1;
  }

  if (usage) {
    a_usage();
    exit(EXIT_FAILURE);
  }
  
  if (encrypt) {
    a_encrypt(mul, add);
  } else {
    a_decrypt(calculate_inverse(mul), add);
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

/*
 * Classical iterative Euclidean algorithm.
 */
int gcd(int m, int n)
{
  int r;
  
  while (n != 0) {
    r = modulo(m, n);
    m = n;
    n = r;
  }

  return m;
}

int calculate_inverse(int m) {
  int inv;
  for (inv = 2; inv <=255; inv++) {
    if (modulo(m * inv, 255) == 1) {
      return inv;
    }
  }
  abort_msg("Could not calculate inverse.");
  return 0;
}

void a_encrypt(int mul, int add)
{
  int cleartext;
  int cyphertext;
  
  while ((cleartext=getchar()) != EOF) {
    cyphertext = modulo(mul * cleartext + add, 255);
    putchar(cyphertext);
  }
}

void a_decrypt(int mul, int add)
{
  int cleartext;
  int cyphertext;
  
  while ((cyphertext=getchar()) != EOF) {
    cleartext = modulo(mul * (cyphertext - add), 255);
    putchar(cleartext);
  }
}

void a_usage()
{
  puts("Usage: affine [-d|-e] -m MUL -a ADD");
  puts("\t-d decrypt stdin to stdout.");
  puts("\t-e encrypt stdin to stdout.");
  puts("\t-m MUL multiplier, where 2 <= MUL <= 255, and gcd(MUL,255) = 1.");
  puts("\t-a ADD addition, where 0 <= ADD <= 255.");
}

void log_msg(char abort_flag, char *title, char *msg)
{
  fprintf(stderr, "%s: %s\n", title, msg);
  if (abort_flag) {
    exit(EXIT_FAILURE);
  }
}

void error_msg(char *msg) {
  log_msg(0, "Error", msg);
}

void abort_msg(char *msg) {
  log_msg(1, "Abort", msg);
}

