//antonino dimaggio
struct Node
{
    int val;
    struct Node* left;
    struct Node* right;
};

int compare(int left, int right);

struct Node* create_node(int val);

struct Node* insert_node(struct Node *root, int val);

void recursive_free(struct Node *root);

void traverse(struct Node *root);
