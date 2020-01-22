#define _GNU_SOURCE
#define ARR_CONSTANT 1000
#include <stdio.h>
#include <stdlib.h>
#include "third.h"

//antonino dimaggio

int search_count = 0;
int collision_count = 0;
struct Hash* hash_arr[ARR_CONSTANT];

void process_file(char *file_name);

int main(int argc, char **argv) {
  process_file(argv[1]);
  printf("%d\n", collision_count);
  printf("%d\n", search_count);
}

void process_file(char *file_name) {
  FILE * fptr;
  char single_line[150];
  fptr = fopen(file_name, "r");

  /* keep looping untill you get till the end of the file */
  while (fgets(single_line, 150, fptr)) {
    if (single_line[0] == 'i') {
      insert(atoi(&single_line[2]));
    } else
    {
      search(atoi(&single_line[2]));
    }
  }
  fclose(fptr);
}

int hash_func(int key) {
  if(key < 0)
    key = abs(key);

  return key % ARR_CONSTANT;
}

void insert(int value) {
  int key_value = hash_func(value);
  struct Hash *new_hash = (struct Hash*) malloc(sizeof(struct Hash));
  if(hash_arr[key_value] == NULL){
    new_hash -> key = key_value;
    new_hash -> value_node = create_node(value);
    hash_arr[key_value] = new_hash;
  }else{
    sorted_insert(&(hash_arr[key_value] -> value_node), create_node(value));
    collision_count ++;
  }
}

void search(int value) {
  int key_value = hash_func(value);
  //now search the linked list
  struct Hash *temp = hash_arr[key_value];
  //checking to see if the head is the value we are searching for
  if(temp != NULL && temp -> value_node -> data == value){
    search_count ++;
  }else{
    struct Node* holder = temp -> value_node -> next;
    while(holder != NULL)
    {
      if(holder -> data == value)
      {
        holder = holder -> next;
        search_count ++;
        return;
      }
      else
      {
         holder = holder -> next;
      }
    }
  }
}

struct Node *create_node(int data)
{
  struct Node* create_node = (struct Node*) malloc(sizeof(struct Node));
  create_node -> data = data;
  create_node -> next = NULL;
  return create_node;
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
      if(to_app -> next != NULL && to_app -> next -> data == create_node -> data)
          return;
    }
    create_node -> next = to_app -> next;
    to_app -> next = create_node;
  }
}


void print_list(struct Node *head) {
  struct Node *hold_node = head;
  printf("   %s", "{Linked List} --> ");
  while(hold_node != NULL)
  {
     printf("%d ", hold_node -> data);
     hold_node = hold_node->next;
  }
  printf("%s", "\n");
}
