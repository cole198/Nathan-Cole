#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_of_list.h"
#include "shell_list.h"
static Node* pop(Node** head);
static void reverse_list(Node** head);

static Node* pop(Node** head) {
	Node* tmp = *head;
	if(tmp != NULL) {
		*head = tmp->next;
		tmp->next = NULL;
	}
	return tmp;
}

static void reverse_list(Node** head) {
	Node* prev = NULL;
	Node* next = NULL;
	Node* curr = *head;
	while(curr != NULL) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*head = prev;

}
Node *List_Load_From_File(char *filename) {
	FILE* fp;
	long int len;
	Node *list;
	Node *tmp;
	long int size;
	fp = fopen(filename, "rb");
	if(fp == NULL) {
		return NULL;
	}
	long int *num = malloc(sizeof(long));
	*num = 0;
	

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	size = len / sizeof(long);
//	list = malloc(size * sizeof(Node));
	fseek(fp, 0, SEEK_SET);
	list = (Node*)malloc(sizeof(Node));
//	printf("%d\n",sizeof(Node*));
	fread(num, sizeof(long), 1, fp);
	list->value = *num;
	list->next = NULL;
//	printf("%ld\n",list->value);
	tmp = list;
	int i = 1;
	while(i < size) {
		fread(num, sizeof(long), 1, fp);
		tmp->next = malloc(sizeof(Node));
		tmp = tmp->next;
		tmp->value = *num;
//		printf("%ld\n",tmp->value);
		tmp->next = NULL;
		i++;
	}
	fclose(fp);
	free(num);
	return list;
}

int List_Save_To_File(char *filename, Node *list) {
	FILE *fp;
	fp = fopen(filename, "wb+");
	if(fp == NULL) {
		return -1;
	}
	int num_written = 0;
	Node *tmp = list;
	while(tmp != NULL) {
	//	num_written += 1;
	//	printf("%ld\n",tmp->value);
		num_written += fwrite(&(tmp->value), sizeof(long), 1, fp);
		tmp = tmp->next;
	}
	fclose(fp);
	return num_written;
//	printf("\n%d",nums);
}

Node *List_Shellsort(Node *list, long *n_comp) {
//	return list;
	int num_elements = 0;
	if(list == NULL) {
		return NULL;
	}
	Node *tmp = list;
//	Node *tmp2 = list;
//	printf("\n%ld\n",list->value);
	// printf("input\n");
	while(tmp != NULL) {
		//printf("%ld\n",tmp->value);
		tmp = tmp->next;
		num_elements++;
		
	}
	// printf("\nnum elements: %d\n",num_elements);
	long interval = 1;
	while(interval < num_elements / 3) {
		interval = interval*3 + 1;
	}
	while(interval > 0) {
		List *sub;
		sub = malloc(sizeof(List));
		sub->next = NULL;
		List *sub_tmp;
		sub_tmp = sub;
		for(int i = 1; i < interval; i++) {
			sub_tmp->next = malloc(sizeof(List));
			sub_tmp = sub_tmp->next;
			sub_tmp->next = NULL;
			sub_tmp->node = NULL;
	//		printf("\n%d\n",i);
		}
		//printf("before reverse: %ld\n",list->value);
	
		reverse_list(&list);
		//printf("after reverse: %ld\n",list->value);
		sub_tmp->next = sub;
		
		tmp = list;
		sub_tmp = sub;
				
				for(int i = 0; i < interval; i ++) {
				
//				while(sub_tmp->node->next != NULL) {
//					sub_tmp ->node  = sub_tmp->node->next; }
				// sub_tmp->node = malloc(sizeof(Node));
				sub_tmp->node = pop(&list);
				//tmp = tmp->next;
				//sub_tmp->node->next = NULL;

				//sub_tmp->node = sub_tmp->node->next;

				//printf("\n%ld",sub_tmp->node->value);
//				tmp = tmp->next;
				sub_tmp = sub_tmp->next;
			//	sub_tmp->node = sub_tmp->node->next;
		}
		
		// printf("\naaaa %ld\n", sub_tmp->node->value);
		for(int i = 0; i < num_elements - interval; i++) {
			//Node* new_node = malloc(sizeof(Node));
			Node* new_node = pop(&list);
			//new_node->value = tmp->value;
			// long x = tmp->value;
			// new_node->next = NULL;
			//printf("newnode: %ld\n org: %ld\n",new_node->value,sub_tmp->node->value);
			*n_comp += 1;
			if((new_node->value) < (sub_tmp->node->value)) {
				
				new_node->next = sub_tmp->node;
				sub_tmp->node = new_node;
				// printf("\nTYEAFF\n");
			}
			else {
				*n_comp += 1;
				new_node->next = sub_tmp->node->next;
				sub_tmp->node->next = new_node;
				Node *ref = sub_tmp->node;
				while(new_node->next != NULL && (new_node->value > new_node->next->value)) {
					*n_comp += 1;
					Node *tmp2 = new_node->next->next;
					ref->next = new_node->next;
					new_node->next->next = new_node;
					new_node->next = tmp2;
					ref = ref->next; 
				}
			}
			//tmp = tmp->next;
			sub_tmp = sub_tmp->next;
		}
		// while(sub_tmp != sub) {
		// 	sub_tmp = sub_tmp ->next; }
		
		sub_tmp = sub;

		list = sub_tmp->node;
		tmp = list;
		sub_tmp->node = sub_tmp->node->next;
		sub_tmp = sub_tmp->next;
//		Node** ref2 = &list;
		for(int i = 1; i < num_elements; i++) {
			list->next = sub_tmp->node;
			sub_tmp->node = sub_tmp->node->next;
			sub_tmp = sub_tmp->next;
			list = list->next;
		}

		for(int i = 0; i < interval; i++) {
			List* del_tmp = sub_tmp;
			sub_tmp = sub_tmp->next;
			free(del_tmp);
		}
//		tmp = list;
// 		while(sub_tmp->node != NULL) {
// 			list = sub_tmp->node;
// //			printf("\n:%ld",list->value);
// 			Node* tmp2 = sub_tmp->node->next;
// //			sub_tmp->node = sub_tmp->node->next;
// //			sub_tmp = sub_tmp->next;
// 			list = list->next;
// 			free(sub_tmp->node);
// 			sub_tmp->node = tmp2;
// 			sub_tmp = sub_tmp->next;
// 		}

	//	while(sub_tmp != NULL) {
	//		List *tmp3 = sub_tmp->next;
	//		free(sub_tmp); 
	//		printf("a");
	//		sub_tmp = tmp3;
	//	}//		printf("after\n%ld\n",sub_tmp->next->node->next->value);	
//		while(sub->node!=NULL) {
///			printf("\naaa %ld bbb\n",sub->node->value);
//			sub = sub->next; }
//		while(sub_tmp != sub) {
//			sub_tmp = sub_tmp->next; }
	//	sub_tmp = sub	;
///		for(int i = 0; i < 3; i++) {
//			printf("\n\n%ld",sub_tmp->node->value);
//			sub_tmp->node = sub->node->next;
//			sub = sub->next; 
//		}

		list = tmp;
	
		// interval = 0 ;

		// while(sub_tmp != NULL) {
		// 	List** del_tmp = &sub_tmp;
		// 	sub_tmp = sub_tmp->next;
		// 	free(*del_tmp);
		// 	*del_tmp = NULL;
		// }

 		interval = (interval - 1) / 3;
		
	}

	return tmp;
}

