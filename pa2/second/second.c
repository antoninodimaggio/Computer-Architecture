#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

//author: antonino dimaggio
//date: 3/3/2019
//modified: 3/3/2019

void ret_parity(unsigned short input);

int main(int argc, char **argv) {
  ret_parity(atoi(argv[1]));
  return 0;
}


void ret_parity(unsigned short input) {
  int go_to_counter = 0;
  int count_ones = 0;
  int pairs  = 0;
  int hold_last_bit = 0;

  for(int i = 0; i < 32; i++)
  {
    if(go_to_counter == 0)
    {
      hold_last_bit = (input >> i) & 1;
      if(hold_last_bit & 1)
        count_ones ++;
      go_to_counter ++;
    }
    else
    {
      int temp_hold = (input >> i) & 1;
      if((temp_hold & 1))
        count_ones ++;

      if(temp_hold & hold_last_bit)
      {
        pairs ++;
        hold_last_bit = 0;
        go_to_counter = 0;
      }
      else
      {
        hold_last_bit = temp_hold;
      }
    }
  }
  if((count_ones % 2) == 0)
    printf("%s\t%d\n", "Even-Parity", pairs);
  else
    printf("%s\t%d\n", "Odd-Parity", pairs);
}
