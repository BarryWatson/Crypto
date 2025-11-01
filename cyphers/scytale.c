A/*
 * scytale.c
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
 * A simple implementation of the σκυτάλη (scytale) transposition cypher.
 * Try
 *    echo  "Hello World 123" | ./scytale -e -w4
 *    echo  "Hello World 123" | ./scytale -e -w4  | ./scytale -d -w4
 *
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFLEN 128 

void sc_usage (void);
void sc_encrypt(unsigned int width, char *buffer, size_t len);
void sc_decrypt(unsigned int width, char *buffer, size_t len);

int main(int argc, char ** argv)
{
  char buff[BUFFLEN];
  char encrypt = 0;
  char decrypt = 0;
  int width = -1;
  char usage = 0;
  char ch;
  
  // Get options
  while ((ch = getopt(argc, argv, "edw:")) != -1) {
    switch (ch) {
    case 'e':
      encrypt = 1;
      break;

    case 'd':
      decrypt = 1;
      break;

    case 'w':
      width = atoi(optarg);
      break;
      
    case '?':
      usage = 1;
      break;
    }
  }
  // Validate input
  if ((encrypt ^ decrypt) == 0) { // One and only one must be set
    usage = 1;
  }

  if (width >= BUFFLEN || width <= 0) {
    usage = 1;
  }

  if (usage) {
    sc_usage();
    exit(-1);
  }
  
  // Perform instructed task
  while (fgets(buff, BUFFLEN, stdin)) {
      if (encrypt) {
	sc_encrypt(width, buff, strlen(buff) - 1); // -1 to ignore terminal '\n'
      }
      else {
	sc_decrypt(width, buff, strlen(buff) - 1); // -1 to ignore terminal '\n'
      }
    }
}

void sc_encrypt(unsigned int width, char *buffer, size_t len)
{
  unsigned int w;
  size_t i;

  for (w=0; w<width; w++) {
    for (i=w; i<len; i=i+width) {
      putchar(buffer[i]);
    }
  }
  putchar('\n');
}

void sc_decrypt(unsigned int width, char *buffer, size_t len)
{
  unsigned int increment;
  unsigned int i;
  unsigned int j;
  
  increment = len/width + ((len % width) ? 1 : 0);

  for (i=0; i < increment; i++) {
    for(j=i; j < len; j += increment) {
      putchar(buffer[j]);
    }
  }
  putchar('\n');
}

void sc_usage()
{
  puts("scytale [-d|-e] -w WIDTH");
  puts("\t-d decrypt stdin to stdout.");
  puts("\t-e encrypt stdin to stdout.");
  puts("\t-w WIDTH the width of the scytale.");
}
