#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//antonino dimaggio

void process_word(char str1[]);

int main(int argc, char **argv){
  for(int i = 1; i < argc; i++)
    process_word(argv[i]);
}

void process_word(char str1[]) {

  for(int i = 0; i < strlen(str1); i++){
    char vow = str1[i];
    if (vow == 'a' || vow == 'A' || vow == 'e' || vow == 'E' || vow == 'i' || vow == 'I' || vow =='o' || vow=='O' || vow == 'u' || vow == 'U'){
      printf("%c", str1[i]);
    }
  }
}
