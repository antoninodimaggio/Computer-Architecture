#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//antonino dimaggio

void processfile(char *file_name);
void print_array(int arr_even[], int arr_odd[], int size_even, int size_odd);
void sort_even(int arr[], int size);
void sort_odd(int arr[], int size);

int main(int argc, char **argv)
{
   processfile(argv[1]);
   return 0;
}

void processfile(char *file_name)
{

  if(access(file_name, F_OK ) != -1 ) {
    //file exists
  } else {
    printf("%s", "error");
    exit(1);
  }

  FILE * fptr;
  fptr = fopen(file_name, "r");

  int val = 0;
  int even_arr [100];
  int odd_arr [100];
  int even_count = 0;
  int odd_count = 0;
  int count = 0;

  while (!feof(fptr)) {
    fscanf(fptr, "%d", &val);
    if (count == 0)
    {
      count++;
    }
    else
    {
      if (val % 2 == 0) {
          even_arr[even_count] = val;
          even_count ++;
        }
      else
        {
          odd_arr[odd_count] =  val;
          odd_count ++;
        }
      }
    }
  sort_even(even_arr, even_count);
  sort_odd(odd_arr, odd_count);
  print_array(even_arr, odd_arr, even_count, odd_count);
  fclose(fptr);
}

void sort_even(int arr[], int size){
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (arr[j] > arr[i])
      {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
      }
    }
  }
}

void sort_odd(int arr[], int size) {
  for (int i = 0; i < size; i++)
	{
		for (int j = 0; j <size; j++)
		{
			if (arr[j] < arr[i])
			{
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void print_array(int arr_even[], int arr_odd[], int size_even, int size_odd) {
  for(int i = 0; i < size_even; i++){
      printf("%d\t", arr_even[i]);
  }
  for(int j = 0; j < size_odd; j++){
      printf("%d\t", arr_odd[j]);
  }
  printf("%s", "\n");
}
