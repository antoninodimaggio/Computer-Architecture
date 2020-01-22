//antonino dimaggio
struct Node
{
  int data;
  struct Node *next;
};

struct Node * create_node(int data);

int delete_node(struct Node** head, int value);

void sorted_insert(struct Node** head, struct Node* create_node);

void print_list(struct Node *n);
