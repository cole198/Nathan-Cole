#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

unsigned char read_bit(int bit, int byte) {
	int mask = 1 << bit;
	byte = byte & mask;
	byte = byte >> bit;
	return byte; 
}

void print_topology(unsigned char* topology, int size, char* output) {
	FILE* fp;
	fp = fopen(output, "w+");
	if(fp == NULL) {
		return;
	}
	int last_bit = 0;
	char zero = '0';
	char one = '1';
	for(int i = 0; i < size - 1; i++) {
		int byte = topology[i];
		for(int j = last_bit; j < 8; j++) {
			unsigned char bit = read_bit(j, byte);
			if(bit == 0) {
				fwrite(&zero, sizeof(char), 1, fp);
				last_bit = 0;
			}
			else {
				fwrite(&one, sizeof(char), 1, fp);
				unsigned char out= get_ascii(topology[i], topology[i+1], (j + 1) % 9);
				last_bit = (j + 1) % 8;
				if(last_bit == 0) {
					i++;
				}
				j = j + 8;
				fwrite(&out, sizeof(char), 1, fp);
			}
		}
	}
	fclose(fp);
}

TreeNode* create_node(int val, TreeNode* prev) {
	TreeNode* Node;
	Node = malloc(sizeof(*Node));
	if(Node == NULL) {
		return NULL; }
	*Node = (TreeNode) {.character = val, .prev = prev, .left = NULL, .right = NULL};
	return Node;
}

TreeNode* create_tree(unsigned char* topology, long size) {
	int last_bit = 0;
	TreeNode* Node = NULL;
	TreeNode* Node_Prev = NULL;
	for(int i = 0; i < size - 1; i++) {
		int byte = topology[i];
		for(int j = last_bit; j < 8; j ++) {
			int bit = read_bit(j, byte);
			if(bit == 0) {
				last_bit = 0;
				Node = create_node(-1, Node_Prev);
				if(Node_Prev != NULL) {
					if(Node_Prev->left == NULL) {
						Node_Prev->left = Node; 
					}
					else {
						Node_Prev->right = Node;
					}

				}
				Node_Prev = Node;
			}
			else {
				Node = create_node(get_ascii(topology[i], topology[i+1], (j + 1) % 9), Node_Prev);
				if(Node_Prev != NULL) {
					if(Node_Prev->left == NULL) {
						Node_Prev->left = Node;
					}
					else {
						Node_Prev->right = Node;
					}
				}
				last_bit = (j + 1) % 8;
				if(last_bit == 0) {
					i++;
				}
				j = j + 8;
				while((Node_Prev != NULL) && (Node_Prev->right != NULL)) {
					Node_Prev = Node_Prev->prev;
				}
			}
		}
	}
	while(Node->prev != NULL) {
		Node = Node->prev;
	}
	return Node;
}

unsigned char get_ascii(unsigned char a, unsigned char b, int bit_num) {
	unsigned char first = ((0xff << bit_num) & a) >> bit_num;
	unsigned char second = ((0xff >> (8 - bit_num)) & b) << (8 - bit_num);
	first = first + second;
	return first;
}

void destroy_tree(TreeNode* Head) {
	if(Head != NULL) {
		destroy_tree(Head->left);
		destroy_tree(Head->right);
		free(Head);
	}
}

long* decode(TreeNode* tree, unsigned char* encoded, long com_bytes, long uncom_bytes, FILE* output, long* num_bits) {
	TreeNode* ref = tree;
	int bit_num = 0;
	int bytes_writ = 0;
	long* freq = NULL;
	if(output != NULL) {
		freq = malloc(sizeof(long) * 256);
		for(int i = 0; i < 256; i ++) {
			freq[i] = 0;
		}
	}
	for(int i = 0; i < uncom_bytes; i++) {
		int byte = encoded[i];
		for(int j = bit_num; j < 8; j++) {
			int bit = read_bit(j, byte);
			*num_bits = *num_bits + 1;
			if(bit == 0) {
				if(ref->left != NULL) {
					ref = ref->left;
				}
				
			}
			else {
				if(ref->right != NULL) {
					ref = ref->right;
				}
				
			}
			if(((ref->character) != -1) && (bytes_writ < uncom_bytes)) {
				unsigned char out = ref->character;
				if(output != NULL) {
					fwrite(&(out), sizeof(char), 1, output);
					freq[out]++;
				}
				ref = tree;
				bytes_writ++;
			}
			if(bytes_writ == uncom_bytes) {
				return freq;
			}
		}
	}
	return freq;
}

TreeNode* pq_enqueue(TreeNode** head, TreeNode* new_node) {
	if(*head == NULL || ((new_node->freq) < ((*head)->freq))) {
		*head = new_node;
	}
	else {
		new_node->next = (*head)->next;
		(*head)->next = new_node;
		TreeNode* ref = *head;
		while((new_node->next != NULL) && ((new_node->freq) >= (new_node->next->freq))) {
			TreeNode* tmp = new_node->next->next;
			ref->next = new_node->next;
			new_node->next->next = new_node;
			new_node->next = tmp;
			ref = ref->next;
		}
	}
	return new_node;
}

TreeNode* pq_dequeue(TreeNode** head) {
	if((*head) == NULL) {
		return NULL; }
	TreeNode* removed_node = *head;
	*head = (*head)->next;
	removed_node->next = NULL;
	return removed_node;
}

TreeNode* create_huff_tree(TreeNode* head) {
	if(head == NULL) {
		return NULL; }

	TreeNode* root = NULL;
	if(head->next == NULL) {
		return head;	
	}	
	while((head->next != NULL) && (head != NULL)) {
		root = (TreeNode*) malloc(sizeof(*root));
		TreeNode* t1 = pq_dequeue(&head);
		TreeNode* t2 = pq_dequeue(&head);
		long new_freq = (t1->freq) + (t2->freq);
		*root = (TreeNode) {.character = -1, .next = head, .freq = new_freq, .left = t1, .right = t2};
		pq_enqueue(&head, root);
	}
	return root; 
}

TreeNode* make_huff_pq(long* freq) {
	TreeNode* head = NULL;
	for(int i = 0; i < 256; i++) {
		if(freq[i] != 0) {
			TreeNode* root = malloc(sizeof(*root));
			*root = (TreeNode) {.character = i, .next = head, .freq = freq[i], .left = NULL, .right = NULL };
			pq_enqueue(&head, root);
		}
	}
	return head;
}

long topology_from_tree(TreeNode* head, FILE* output) {
	if(head == NULL) {
		return 0; }
	long bit_count = 0;
	char zero = '0';
	char one = '1';
	if(head->character == -1) {
		fwrite(&zero, sizeof(char), 1, output);
		bit_count++;
		bit_count += topology_from_tree(head->left, output);
		bit_count += topology_from_tree(head->right, output);
	}
	else {
		fwrite(&one, sizeof(char), 1, output);
		bit_count++;
		unsigned char out = head->character;
		bit_count += 8;
		fwrite(&out, sizeof(char), 1, output);
	}
	return bit_count;
}


