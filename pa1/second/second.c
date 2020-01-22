#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "second.h"

//antonino dimaggio

void file_reader(char *file_name, struct Node *head);

int main(int argc, char **argv)
{
  struct Node* head_node = NULL;
  file_reader(argv[1], head_node);
  return 0;
}

void file_reader(char *file_name, struct Node *head_node)
{
  FILE * fptr;
  fptr = fopen(file_name, "r");
  char single_line[150];
  int count = 0;

  while (fgets(single_line, 150, fptr)) {
    //inserting node which allocates mem
    if (single_line[0] == 'i') {
      struct Node *app_node = create_node(atoi(&single_line[2]));
      sorted_insert(&head_node, app_node);
      count ++;
    }
    else if(count != 0)
    {
        count = count - delete_node(&head_node, atoi(&single_line[2]));
    }
  }
  fclose(fptr);
  if(count != 0)
  {
    printf("%d\n", count);
    print_list(head_node);
  }
  else
  {
    printf("%d\n", count);
  }
}

void sorted_insert(struct Node** head, struct Node* create_node)
{
  struct Node* to_app;
  /* so this insert is going to run on O(N)*/
  if (*head == NULL || (*head)->data >= create_node->data)
   {
       create_node->next = *head;
       *head = create_node;
   }
  else
  {
    to_app = *head;
    while (to_app -> next != NULL && to_app -> next -> data < create_node -> data)
    {
      to_app = to_app -> next;
    }
    create_node -> next = to_app -> next;
    to_app -> next = create_node;
  }
}

int delete_node(struct Node **head, int value)
{
  struct Node *hold_node = *head, *prev;
  if (hold_node != NULL && hold_node -> data == value){
      *head = hold_node -> next;
      free(hold_node);
      return 1;
  }
  while (hold_node != NULL && hold_node -> data != value)
  {
    prev = hold_node;
    hold_node = hold_node -> next;
  }
  if (hold_node == NULL)
    return 0;
  prev -> next = hold_node -> next;
  free(hold_node);
  return 1;
}

struct Node *create_node(int data)
{
  struct Node* create_node = (struct Node*) malloc(sizeof(struct Node));
  create_node -> data = data;
  create_node -> next = NULL;
  return create_node;
}

void print_list(struct Node *head) {
  struct Node *hold_node = head;
  int hold_data = 0;
  while(hold_node != NULL)
  {
    if (hold_data != hold_node -> data)
    {
      printf("%d\t", hold_node -> data);
      hold_data =  hold_node -> data;
      hold_node = hold_node->next;
    }
    else
    {
      hold_node = hold_node->next;
    }
  }
}
