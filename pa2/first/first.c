#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//author: antonino dimaggio
//date: 3/3/2019
//modified: 3/3/2019

void process_file(char *file_name);

int main(int argc, char **argv) {
  process_file(argv[1]);
  return 0;
}

void process_file(char *file_name)
{
  if(access(file_name, F_OK ) != -1 ) {
    //file exists
  } else {
    printf("%s", "error");
    exit(1);
  }

  FILE * fptr;
  fptr = fopen(file_name, "r");

  char single_line[10];
  unsigned int top_holder = 0;
  int count = 0;

  while (fgets(single_line, 10, fptr)) {

    if(count == 0)
    {
      top_holder = atoi(&single_line[0]);
      count ++;
    }

    if(single_line[0] == 's')
    {
      int nth_bit;
      int set_val;
      nth_bit = atoi(&single_line[4]);
      set_val = atoi(&single_line[6]);
      if(set_val == 0)
        top_holder &= ~(1 << nth_bit);
      else
        top_holder |= (set_val << nth_bit);
      printf("%d", top_holder);
      printf("%s", "\n");
    }

    if(single_line[0] == 'g')
    {
      //ignore second value
      int nth_bit_get;
      unsigned int temp_top_holder;
      nth_bit_get = atoi(&single_line[4]);
      temp_top_holder = (top_holder >> nth_bit_get) & 1;
      printf("%d", temp_top_holder);
      printf("%s", "\n");
    }

    if(single_line[0] == 'c')
    {
      //ignore second value
      int nth_bit_comp;
      nth_bit_comp = atoi(&single_line[4]);
      top_holder ^= (1 << nth_bit_comp);
      printf("%d", top_holder);
      printf("%s", "\n");
    }
  }
  fclose(fptr);
}
