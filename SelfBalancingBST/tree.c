#include <stdio.h>
#include <stdlib.h>
#include "hbt.h"
#include "htree.h"

int get_height(Tnode* root)  {
	//printf("here");
	if(root == NULL) {
		return 0;
	}
	int height = 1;
	int x = get_height(root->left);
	int y = get_height(root->right);

	height += x > y ? x : y;

	return height;
}

int check_balance(Tnode* root) {
	if(root == NULL) {
		return 0;
	}
	int bal = 0;
	if(root->balance > 1 || root->balance < -1) {
		bal += 1;
	}
	bal += check_balance(root->left);
	bal += check_balance(root->right);

	return bal;
}

Tnode* create_tree(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	long int num = ftell(fp) /(sizeof(int) + sizeof(char));
	fseek(fp,0,SEEK_SET);
	Tnode* head = NULL;
	for(int i = 0; i < num; i++) {
//		Tnode* new_node = malloc(sizeof(new_node));
		int key = 0;
		char op = 0;
		fread(&key,sizeof(int), 1, fp);
		
		fread(&op,sizeof(char),1,fp);
//		int bal = 0;
//		new_node = (Tnode) {.left = NULL, .right = NULL, .key = key, .balance = bal};
//		Tnode* new_node = create_node(key);
//		if(head == NULL) {
		//printf("key:%d op: %c\n",key,op);
		if(op == 'i') {
			insert(&head, key);
		}
		else if(op == 'd') {
			delete(&head, key, NULL);
		}
		



	}
	return head;
}

Tnode* create_node(int key) {
	Tnode* new_node = malloc(sizeof(*new_node));
	*new_node = (Tnode) {.left = NULL, .right = NULL, .key = key, .balance = 0};
	return new_node;
}

void insert(Tnode** head, int key) {
	Tnode* prev = NULL;
	Tnode* curr = *head;
	Tnode* new_node = NULL;
	Tnode* ya = *head;
	Tnode* pya = NULL;
	while(curr != NULL) {
		prev = curr;

		if(key <= curr->key){
			new_node = curr->left;
		}
		else {
			new_node = curr->right;
		}
		if(new_node!= NULL && new_node->balance != 0) {
			pya = curr;
			ya = new_node;
		}
		curr = new_node;
	}
	new_node = create_node(key);

	if(prev == NULL) {
		*head = new_node;
		return;
	}
	else {
		if(key <= prev->key) {
			prev->left = new_node;
		}
		else {
			prev->right = new_node;
		}
	}
	curr = ya;
	while(curr != new_node) {
		if(key <= curr->key) {
			curr->balance += 1;
			curr = curr->left;
		}
		else {
			curr->balance -= 1;
			curr = curr->right;
		}
	}
	if((ya->balance < 2) && (ya->balance > -2)) {
		return;
	}
	Tnode* child = NULL;
	if(key <= ya->key) {
		child = ya->left;
	}
	else {
		child = ya->right;
	}
	
	if((ya->balance == 2) && (child->balance == 1)) {
		curr = CR(ya);
		ya->balance = 0;
		child->balance = 0;
	}
	else if((ya->balance == -2) && (child->balance == -1)) {
		curr = CCR(ya);
		ya->balance = 0;
		child->balance = 0;
	}
	else if((ya->balance ==2) && (child->balance == -1)) {
		ya->left = CCR(child);
		curr = CR(ya);
		if(curr->balance == 0) {
			ya->balance = 0;
			child->balance = 0;
		}
		else {
			if(curr->balance == 1) {
				ya->balance = -1;
				child->balance = 0;
			}
			else {
				ya->balance = 0;
				child->balance = 1;
			}

		}
		curr->balance = 0;
	}
	else if((ya->balance == -2) && (child->balance == 1)) {
		ya->right = CR(child);
		curr = CCR(ya);
		if(curr->balance == 0) {
			ya->balance = 0;
			child->balance = 0;
		}
		else {
			if(curr->balance == -1) {
				ya->balance = 1;
				child->balance = 0;
			}
			else {
				ya->balance = 0;
				child->balance = -1;
			}
			curr->balance = 0;
		}
	}
	if(pya == NULL) {
		*head = curr;
	}
	else {
		if(key <= pya->key) {
			pya->left = curr;
		}
		else {
			pya->right = curr;
		}
	}

}

int delete(Tnode** root, int key, Tnode* prev) {
	if(*root == NULL) {
		
		return 0;
	}
	//printf("called with: %d\n",(*root)->key);
	
	//printf("here  %d\n\n",(*root)->balance);
	// Tnode* old_gpa = gpa;
	// gpa = prev;
	int change = 0;
	int bal = (*root)->balance;
	//int left_bal = 0;
	//int right_bal = 0;
	//int pred_flag = 0;
	if(((*root)->key == key)) {
		
		//printf("Deleting: %d\n",(*root)->key);
		change = 1;
		// if(key < (*root)->key) {
		// 	(*root)->balance -= 1;
			
		// 	//if(prev->balance == 0) {
		// 		//(*root)->balance -= 1;
		// 	//}
		// }
		// else {
		// 	(*root)->balance += 1;
		// 	//if(prev->balance == 0) {
		// 		//(*root)->balance +=1;
		// 	//}
		// }
		if(((*root)->left == NULL) && ((*root)->right == NULL)) {
			//printf("heeeeee\n");
			if(prev!= NULL) {
				if(*root == prev->left) {
					//printf("decrement\n");
					prev->balance -= 1;
				}
				else if(*root == prev->right) {
					prev->balance += 1;
					//printf("increment\n");
				}
			}
			free(*root);
			*root = NULL;
			
			//prev->right = NULL;
			//prev->left = NULL;
		}
		else if(((*root)->left != NULL) && ((*root)->right == NULL)) {
			//printf("deleting: %d\n",(*root)->key);
			//left_bal = (*root)->left->balance;
			(*root)->key = (*root)->left->key;
			(*root)->balance = 0;
			free((*root)->left); 
			(*root)->left = NULL;
			// if(prev != NULL) {
			// 	prev->balance -= 1;
			// }
			// Tnode* tmp = (*root)->left;
			// //*root = (*root)->left;
			
			// if(key < prev->key) {
			// 	printf("here %d: %d\n",prev->key, tmp->key);
			// 	prev->left = tmp;
			// 	printf("left: %d\n",prev->left->key);
			// 	printf("prev: %d\n",prev->key);
			// 	printf("right: %d\n",prev->right->key);
			// }
			// else {
			// 	prev->right = (*root);
			// }
			// free(tmp);
			// tmp = NULL;
			
			
			//printf("left: %d\n",prev->left->key);
		}
		else if(((*root)->left == NULL) && ((*root)->right != NULL)) {
			//right_bal = (*root)->right->balance;
			(*root)->key = (*root)->right->key;
			(*root)->balance = 0;
			free((*root)->right);
			(*root)->right = NULL;
			// if(prev != NULL) {
			// 	prev->balance += 1;
			// }
			// Tnode** tmp = root;
			// *root = (*root)->right;
			
			// if(key < prev->key) {
			// 	prev->left = *tmp;
			// }
			// else {
			// 	prev->right = *tmp;
			// }

			// free(*tmp);
			// *tmp = NULL;
		}
		else if(((*root)->left != NULL) && ((*root)->right != NULL))  {
			//left_bal = (*root)->left->balance;
			//right_bal = (*root)->right->balance;
			//change = 0;
			// Tnode* prev_pred = *root;
			Tnode* pred = inorder_pred((*root)->left);
			//printf("found pred %d\n",pred->key);
			int x = pred->key;
			(*root)->key = x;
			
			//pred->key = x == HBT_MAX ? HBT_MIN : HBT_MAX; 
			
			delete(&((*root)->left),pred->key, *root);
			
			//delete(&pred, pred->key, prev_pred);

			//delete_pred(*root, pred, pred->key);
			//delete(&(pred),pred->key, *root);
		}
		
	}
	else if(key <= (*root)->key) {
		change += delete(&((*root)->left), key, *root);
	}
	else {
		change += delete(&((*root)->right),key, *root);
	}

	//print_preorder(*root);
	//printf("root");
	//print_preorder(prev);
	//if((*root) != NULL) {
		//printf("change:%d - key:%d\n",change,(*root)->key);
	//}
	//printf("change:%d - deleted key\n",change);
	if(*root != NULL && change) {
			
		// if((*root)->left != NULL && (left_bal != (*root)->left->balance)) {
		// 	(*root)->balance 

		// }
		// else if((*root)->right != NULL && (right_bal != (*root)->right->balance)) {

		// }
		
	 	Tnode* curr = NULL;
	 	Tnode* child = NULL;
		//Tnode* o_child = NULL;
		//if(key < (*root)->key) {
		if((*root)->balance == -2) {
			child = (*root)->right;
			//o_child = (*root)->left;
		}
		else if((*root)->balance == 2) {
			child = (*root)->left;
			//o_child = (*root)->right;
		}
		int child_bal = 0;
		//int o_child_bal = 0;
		if(child != NULL) {
			child_bal = child->balance;
		}
		// if(o_child != NULL) {
		// 	o_child_bal = o_child->balance;
		// }
		//printf("child of node %d has bal %d\n",(*root)->key,child_bal);
	// 	//printf("check %d",(*root) == NULL);
	// 	//prev->balance = get_height(prev->left) - get_height(prev->right);
		// if (o_child_bal == 0) {
		// 	//printf("child zero at node: %d\n",(*root)->key);
		// 	if(key < (*root)->key) {
		// 		(*root)->balance -= 1;
		// 	}
		// 	else {
		// 		(*root)->balance += 1;
		// 	}
		// }
	// 	printf("Nodes %d \n,",prev->balance);
		if(((*root)->balance == 2)  && (child_bal == 1)) {
			//printf("hre %d: %d\n",(*root)->key, child_bal);
			curr = CR(*root);
			//printf("Curr node:%d\n", curr->key);
			(*root)->balance = 0;
			child->balance = 0;
			//print_preorder(curr);
			*root = curr;
			// if(*root == NULL) {
			// 	printf("yes\n");
			// 	//*root = curr;
			// 	printf("curr: %d\n",curr->left->key);
			// 	//printf("root: %d\n",(*root)->right->key);
			// }
			
			//*orig_root = NULL;
			//*orig_root = curr;
		}
		else if(((*root)->balance == -2) && (child_bal == -1)) {
			//printf("pop\n");
			curr = CCR(*root);
			(*root)->balance = 0;
			child->balance = 0;
			*root = curr;
		}
		else if(((*root)->balance == 2)  && (child_bal == -1)) {
			// printf("yep\n");
			(*root)->left = CCR(child);
			curr = CR(*root);
			if(curr->balance == 0) {
				(*root)->balance = 0;
				child->balance = 0;
			}
			else {
				if(curr->balance == 1) {
					(*root)->balance = -1;
					child->balance = 0;
				}
				else {
					(*root)->balance = 0;
					child->balance = 1;
				}

			}
			curr->balance = 0;
			*root = curr;
		}
		else if(((*root)->balance == -2) && (child_bal == 1)) {
			// printf("nope\n");
			(*root)->right = CR(child);
			curr = CCR(*root);
			if(curr->balance == 0) {
				//printf("nope here 1\n");
				(*root)->balance = 0;
				child->balance = 0;
			}
			else {
				if(curr->balance == -1) {
					//printf("nope here 2\n");
					(*root)->balance = 1;
					child->balance = 0;
				}
				else {
					//printf("nope here 3\n");
					(*root)->balance = 0;
					child->balance = -1;
				}
				// curr->balance = 0;
				// *root = curr;
			}
			curr->balance = 0;
			*root = curr;
	 	}
		else if(((*root)->balance == 2) && child_bal == 0) {
			child = (*root)->left;
			//printf("here PA\n");
			curr = CR(*root);
			//curr->balance = -1;
			(*root)->balance = 1;
			child->balance = -1;
			// if((*root)->left != NULL) {
			// 	(*root)->left->balance = -1;
			// }
			
			*root = curr;

		}		
		//else if(((*root)->balance == -2) && child_bal == 0) {
		else if(((*root)->balance == -2) && child_bal == 0) {
			child = (*root)->right;
			//printf("here PB\n");
		 	curr = CCR(*root);
		 	//curr->balance = -2;
		 	(*root)->balance = -1;
			child->balance = 1;
			*root = curr;
		}
		if((*root)->balance == 0 && prev != NULL) {
			//printf("adjusting bal at node: %d / %d: %d\n", (*root)->key, prev->key,(*root)->balance);
			if(prev->left == (*root)) {
				if(bal == -1) {
					//printf("here up: %d\n",(*root)->key);
					prev->balance -= 1;
			}
				else if(bal == 1) {
					//printf("here down: %d prev: %d\n",(*root)->key,prev->key);
					prev->balance -= 1;
			}

			}
			else if(prev->right == (*root)) {
				if(bal == -1) {
					//printf("here up: %d\n",(*root)->key);
					prev->balance += 1;
			}
				else if(bal == 1) {
					//printf("here down: %d prev: %d\n",(*root)->key,prev->key);
					prev->balance += 1;
			}
			}
			
		}
		
	 }	

	// 	if(curr != NULL) {
	// 		printf("curr again:%d\n",curr->key);
	// 		printf("curr left again:%d\n",curr->left->key);
	// 		printf("curr right again:%d\n",curr->right->key);
	// 		printf("check 1:%d\n",curr->left->left == NULL);
	// 		printf("check 2:%d\n",curr->left->right == NULL);
	// 		printf("check 3:%d\n",curr->right->left == NULL);
	// 		printf("check 4:%d\n",curr->right->right == NULL);
	// 		*root = curr;
	// 		printf("curr again:%d\n",(*root)->key);
	// 		printf("curr left again:%d\n",(*root)->left->key);
	// 		printf("curr right again:%d\n",(*root)->right->key);
	// 		printf("check 1:%d\n",(*root)->left->left == NULL);
	// 		printf("check 2:%d\n",(*root)->left->right == NULL);
	// 		printf("check 3:%d\n",(*root)->right->left == NULL);
	// 		printf("check 4:%d\n",(*root)->right->right == NULL);
	// 	}
	// 	// else if(curr != NULL) {
	// 	// 	//prev->balance += change;
	// 	// 	if(key< prev->key) {
	// 	// 		prev->right = curr;
	// 	// 	}
	// 	// 	else {
	// 	// 		prev->left = curr;
	// 	// 	}
	// 	// }



	// }
	// printf("change%d\n",change);
	// //Tnode* child =
	return change;

}

Tnode* CCR(Tnode* root) {
	Tnode* new_root = root->right;
	root->right = new_root->left;
	new_root->left = root;
	return new_root;
}

Tnode* CR(Tnode* root) {
	Tnode* new_root = root->left;
	root->left = new_root->right;
	new_root->right = root;
	return new_root;
}

void print_preorder(Tnode* root, FILE* fp) {
	if(root == NULL) {
		return;
	}
	//printf("node: %d childs: %d bal:%d\n",root->key, (root->left != NULL)*2 + (root->right != NULL),root->balance);
	if(fp != NULL) {
		int x = root->key;
		int y = (root->left != NULL)*2 + (root->right != NULL);
		fwrite(&x, sizeof(int), 1, fp);
		fwrite(&y, sizeof(char), 1, fp);
	}
	print_preorder(root->left, fp);
	print_preorder(root->right, fp);
}

void destroy_BST(Tnode** root) {
	if((*root) != NULL) {
		destroy_BST(&((*root) -> left));
		destroy_BST(&((*root) -> right));
		free(*root);
		*root = NULL;
	}
}

void destroy_tree(Tnode* root) {
	if(root == NULL) {
		return;
	}
	destroy_tree(root->left);
	destroy_tree(root->right);
	free(root);
}

Tnode* inorder_pred(Tnode* root) {
	Tnode* pred = root;
	//printf("orig pred: %d\n",root->key);
	//while(pred != NULL && pred->right!= NULL) {
	while(pred->right != NULL) {
		//printf(" pred: %d\n",pred->key);
		//prev = pred;
		pred = pred->right;
	}
	pred->right = NULL;
	//printf("Check pred: %d %d\n",pred->left == NULL, pred->right==NULL);
	return pred;
}

Tnode* tree_from_post(FILE* fp, int* bst) {
	fseek(fp, 0, SEEK_END);
	float num = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	num = num / (sizeof(char) + sizeof(int));
	if(num != (int)num) {
		return NULL;
	}
	num = (int)num;
	int* keys = malloc(sizeof(int) * num);
	char* names = malloc(sizeof(char) * num);
	// names = {0};
	// keys = {0};
	//printf("num: %f\n",num);
	for(int i = 0; i < num; i++) {
		keys[i] = 0;
		names[i] = 0;
		fread(&keys[i], sizeof(int), 1, fp);
		fread(&names[i], sizeof(char), 1, fp);
		if(keys[i] > HBT_MAX || keys[i] < HBT_MIN) {
			free(keys);
			free(names);
			return NULL;
		}
	}
	Tnode* root = NULL;
	int i = 0;
	while(i < num) {
		insert_node(&root, keys, names, &i, bst);
		

	}
	//printf("first node: %d %d\n",names[1], keys[1]);
	free(keys);
	free(names);
	return root;
}

// void delete_pred(Tnode* root, Tnode* pred, int key) {
// 	if(root == NULL) {
// 		return;
// 	}
	
// 	if(((root)->key == key) && (root = pred)) {
// 		if(((root)->left == NULL) && ((root)->right == NULL)) {
// 			//printf("heeeeee\n");
			
// 			free(root);
// 			root = NULL;
			
// 		}
// 		else if(((root)->left != NULL) && ((root)->right == NULL)) {
			
// 			(root)->key = (root)->left->key;
// 			(root)->balance = 0;
// 			free((root)->left); 
// 			(root)->left = NULL;
			
// 		}
// 		else if(((root)->left == NULL) && ((root)->right != NULL)) {
			
// 			(root)->key = (root)->right->key;
// 			(root)->balance = 0;
// 			free((root)->right);
// 			(root)->right = NULL;
// 		}
// 		else {
// 			printf("FAILEDED\n");
// 		}
		
// 	}
// 	else if (key <= (root)->key) {
// 		delete_pred(((root)->left), pred, key);
// 	}
// 	else {
// 		delete_pred(((root)->right),pred,key);
// 	}

// }

void insert_node(Tnode** root, int* keys, char* labels, int* i, int* bst) {
	Tnode* node = malloc(sizeof(*node));
	*node = (Tnode) {.key=keys[*i], .left = NULL, .right = NULL};
	//node->key = keys[*i];
	int label = labels[*i];
	*i = *i + 1;
	if(label == 2) {
		insert_node(&(node->left), keys, labels, i, bst);
	}
	else if(label == 3) {
		insert_node(&(node->left), keys, labels, i, bst);
		insert_node(&(node->right), keys, labels, i, bst);
	}
	else if(label == 1) {
		insert_node(&(node->right), keys, labels, i, bst);
	}
	*root = node;
	(*root)->balance = get_height((*root)->left) - get_height((*root)->right);
	if(((*root)->right != NULL) && (*root)->right->key <= (*root)->key) {
		*bst += 1;
	
	}
	else if(((*root)->left != NULL) && (*root)->left->key > (*root)->key) {
		*bst += 1;
	}

}