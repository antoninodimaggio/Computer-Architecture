#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

//antonino dimaggio

void process_file(char *file_name);
void multiply(int **m_arr, int **m_arr_two, int row, int col, int row_two, int col_two);
void free_arr(int **arr, int n) ;

int main(int argc, char **argv) {
  process_file(argv[1]);
  return 0;
}

void process_file(char *file_name)
{
  FILE * fptr;
  fptr = fopen(file_name, "r");
  int val = 0;
  int row = 0;
  int col = 0;
  int row_two = 0;
  int col_two = 0;

  int **m_arr = malloc(100*sizeof(int *));
  int **m_arr_two = malloc(100*sizeof(int *));

  for(int i = 0; i < 100; i++) {
    m_arr[i] = malloc(100 * sizeof(int));
    if(m_arr[i] == NULL) {
        printf("Out of memory\n");
        exit(1);
      }
    }

  for(int i = 0; i < 100; i++) {
    m_arr_two[i] = malloc(100 * sizeof(int));
    if(m_arr_two[i] == NULL) {
        printf("Out of memory\n");
        exit(1);
      }
    }

  fscanf(fptr, "%d", &val);
  row = val;
  fscanf(fptr, "%d", &val);
  col = val;
  for(int i=0 ;i<row; i++)
  {
      for(int j=0; j<col; j++)
        {
          fscanf(fptr, "%d", &val);
          m_arr[i][j] = val;
        }
  }
  fscanf(fptr, "%d", &val);
  row_two = val;

  fscanf(fptr, "%d", &val);
  col_two = val;
  for(int i=0 ;i<row_two; i++)
  {
      for(int j=0; j<col_two; j++)
        {
          fscanf(fptr, "%d", &val);
          m_arr_two[i][j] = val;
        }
  }
  if(col != row_two)
    printf("%s", "bad-matrices");
  else
    multiply(m_arr, m_arr_two, row, col, row_two, col_two);
  fclose(fptr);
  free_arr(m_arr, 100);
  free_arr(m_arr_two, 100);
}

void multiply(int **m_arr, int **m_arr_two, int row, int col, int row_two, int col_two)
{
  int curr_sum = 0;
  for(int i=0; i<row; i++){
    for(int j=0; j<col_two; j++) {
      for(int k=0; k<col; k++)
      {
          curr_sum += m_arr[i][k]*m_arr_two[k][j];
      }
    printf("%d\t", curr_sum);
    curr_sum = 0;
    }
    printf("%s", "\n");
  }
}

void free_arr(int **arr, int n) {
    for (int i = 0; i < n; ++i) {
        free(arr[i]);
    }
    free(arr);
}
