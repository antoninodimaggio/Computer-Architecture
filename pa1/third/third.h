// hash table declarations
//antonino dimaggio
struct Hash {
  int key;
  struct Node *value_node;
};

int hash_func(int key);

void insert(int value);

void search(int value);


// linked list declrations
struct Node
{
  int data;
  struct Node *next;
};

struct Node * create_node(int data);

void sorted_insert(struct Node** head, struct Node* create_node);

void delete_node(struct Node** head, int value);

void print_list(struct Node *head);
