#include "hbt.h"
int get_height(Tnode* root);
int check_balance(Tnode* root);
void insert(Tnode** head, int key);
Tnode* create_node(int key);
Tnode* create_tree(FILE* fp);
Tnode* CCR(Tnode* root);
Tnode* CR(Tnode* root);
void print_preorder(Tnode* root, FILE* fp);
int delete(Tnode** root, int key, Tnode* prev);
void destroy_BST(Tnode** root);
Tnode* inorder_pred(Tnode* root);
Tnode* tree_from_post(FILE* fp, int* bst);
void delete_pred(Tnode* root, Tnode* pred, int key);
void insert_node(Tnode** root, int* keys, char* labels, int* i, int* bst);
void destroy_tree(Tnode* root);


