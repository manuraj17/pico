#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STRING_LENGTH 255
#define MAX_TOKENS 100

#ifndef _tokenizer_h
#define _tokenizer_h

int tokenize(char *line, char ***tokens, char *delimiter);
void print_list(char **list);

#endif
