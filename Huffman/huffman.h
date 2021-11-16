
#include <stdlib.h>
typedef struct Tree_Node {
	int character;
	struct Tree_Node* next;
	long freq;
	struct Tree_Node *prev;
	struct Tree_Node *left;
	struct Tree_Node *right;
} TreeNode;

//typedef struct pq_Node {
//	long freq;
//	struct pq_Node* next;
//} pqNode;

//Node* tree_from_encoded(
unsigned char read_bit(int bit, int byte);
unsigned char get_ascii(unsigned char a, unsigned char b, int bit_num);
void print_topology(unsigned char* topology, int size, char* output);
TreeNode* create_tree(unsigned char* topology, long size);
void destroy_tree(TreeNode* Head);
long* decode(TreeNode* tree, unsigned char* encoded, long com_bytes,long uncom_bytes, FILE* output, long* num_bits);
TreeNode* make_huff_pq(long* freq);
TreeNode* pq_enqueue(TreeNode** head, TreeNode* new_node);
TreeNode* create_huff_tree(TreeNode* head);
long topology_from_tree(TreeNode* head, FILE* output);
TreeNode* pq_dequeue(TreeNode** head);
TreeNode* create_node(int val, TreeNode* prev);
