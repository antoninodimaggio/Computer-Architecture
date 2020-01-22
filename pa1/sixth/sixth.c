#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sixth.h"

//antonino dimaggio

void process_file (char *file_name);

int main(int argc, char **argv)
{
  process_file(argv[1]);
  return 0;
}

void process_file(char *file_name)
{
  FILE * fptr;
  if(access(file_name, F_OK ) != -1 ) {
    //file exists
  } else {
    printf("%s", "error");
    exit(1);
  }
  char singleLine[150];
  fptr = fopen(file_name, "r");
  struct Node *bst = NULL;


  while (fgets(singleLine, 150, fptr)) {
    if (singleLine[0] == 'i' && bst == NULL) {
      bst = create_node(atoi(&singleLine[2]));
    } else
    {
      insert_node(bst, atoi(&singleLine[2]));
    }
  }
  traverse(bst);
  recursive_free(bst);
  return;
}

int compare(int left, int right)
{
  if (left > right)
  {
    return 1;
  }
  if (left < right)
  {
    return -1;
  }
  return 0;
}

struct Node* create_node(int val)
{
  struct Node *noder = (struct Node*) malloc(sizeof(struct Node));
  noder -> val = val;
  noder -> left = NULL;
  noder -> right = NULL;
  return noder;
}


struct Node* insert_node(struct Node *root, int val)
{
  if(root == NULL)
  {
    root = create_node(val);
  }
  else
  {
    int compare_ret = 0;
    int putter = 0;
    struct Node *temp_hold = root;
    struct Node *prev = NULL;
    while(temp_hold != NULL)
    {
      compare_ret = compare(val, temp_hold -> val);
      prev = temp_hold;
      if (compare_ret < 0)
      {
        temp_hold = temp_hold -> left;
        putter = 1;
      }
      if (compare_ret > 0)
      {
        temp_hold = temp_hold -> right;
        putter = 0;
      }
      if (compare_ret == 0)
      {
        return NULL;
      }
    }
    if(putter == 1)
      prev -> left = create_node(val);
    else
      prev -> right = create_node(val);
  }
  return root;
}

void recursive_free(struct Node *root)
{
    if(root != NULL)
    {
        recursive_free(root->left);
        recursive_free(root->right);
        free(root);
    }
}


void traverse(struct Node *root)
{
    if(root == NULL)
      return;

    struct Node *temp_hold = root;

    traverse(temp_hold->left);

    printf("%d\t", temp_hold -> val);

    traverse(temp_hold -> right);
}
