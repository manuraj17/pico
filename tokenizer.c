#include "tokenizer.h"

/* 
 * Function:  tokenize
 * -------------------
 * Tokenize a string to array of tokens as per the delimiter. NULL is appended
 * to the end of the array as a sentinel value.
 * 
 * @param   {char*, char**, char* } String to be split, token's array, delimiter 
 * @return  {int} number of tokens
 *
 */
int tokenize(char *line, char ***tokens, char *delimiter){

  char* buffer;
  int token_count = 0;
  
  buffer    = (char*)  malloc(strlen(line) * sizeof(char));
  (*tokens) = (char**) malloc(sizeof(char**)*MAX_TOKENS);

  // Create a copy of the string as strtok mutates it. 
  strcpy(buffer, line);
  
  (*tokens)[token_count++] = strtok(buffer, delimiter);
  while ((((*tokens)[token_count] = strtok(NULL, delimiter)) != NULL) &&
      (token_count < MAX_TOKENS)) token_count++;
  
  // Add NULL as the last element
  (*tokens)[token_count+1] = NULL;

  return (token_count);
}

/* 
 * Function:  print_list
 * --------------------
 * Output a list of strings. Each strings will be printed 
 * on a new line. We assume the string to have a sentinel value (NULL).
 *
 * @param {char **} array of strings
 * @return 
 *
 */
void print_list(char **list){
  for(int i=0; list[i]; ++i){
    const char *ch = list[i];
    while(*ch) {
      putchar(*ch++);
    }
    putchar('\n');
  }
}
