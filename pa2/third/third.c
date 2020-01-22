#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

//author: antonino dimaggio
//date: 3/3/2019
//modified: 3/7/2019

void det_pally(unsigned short input);

int main(int argc, char **argv) {
  det_pally(atoi(argv[1]));
  return 0;
}


void det_pally(unsigned short input)
{
  //converge from both ends of the list
  int l = 1; //left position
  int r = 16; //right position
  for(int i = 0; i < 16; i++)
   {
      if((1 & (input >> (l-1))) != (1 & (input >> (r-1))))
      {
        printf("%s", "Not-Palindrome\n");
        return;
      }
      l++; r--;
   }
   printf("%s", "Is-Palindrome\n");
}
