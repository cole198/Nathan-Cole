#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"
#include "htree.h"
int main(int argc, char* argv[]) {
	if(!strcmp(argv[1],"-b")) {
		if(argc != 4) {
			printf("%d\n",-1);
			return EXIT_FAILURE;
		}
		Tnode* root = NULL;
		FILE* fp = fopen(argv[2],"rb");
		if(fp == NULL) {
			printf("%d\n",-1);
			return EXIT_FAILURE;
		}
		FILE* output = fopen(argv[3], "wb+");
		root = create_tree(fp);
		if(root == NULL) {
			printf("%d\n",0);
			fclose(fp);
			fclose(output);
			return EXIT_FAILURE;
		}
		else {
			printf("%d\n",1);

		}
		print_preorder(root, output);
		destroy_BST(&root);
		fclose(fp);
		fclose(output);
	}
	else if(!strcmp(argv[1],"-e")) {
		if(argc != 3) {
			printf("%d,%d,%d\n",-1,0,0);
			return EXIT_FAILURE;
		}
		FILE* fp = fopen(argv[2], "rb");
		if(fp == NULL) {
			printf("%d,%d,%d\n",-1,0,0);
		}
		int bst = 0;
		Tnode* root = tree_from_post(fp, &bst);
		if(root == NULL) {
			fclose(fp);
			printf("%d,%d,%d\n",0,0,0);

		}
		//printf("height: %d\n",get_height(root));

		int balanced = 0;
		balanced = check_balance(root);
		
		printf("%d,%d,%d\n",1,bst == 0,balanced == 0);
		
		fclose(fp);
		destroy_tree(root);
	}
	
	
	
	// insert(&root, 3);
	// insert(&root, 3);
	// insert(&root, 3);
	// insert(&root, 3);

	// insert(&root, 1);
	// insert(&root, 1);
	// insert(&root, 1);
	// insert(&root, 1);

	// insert(&root, 2);
	// insert(&root, 2);
	// insert(&root, 2);
	// insert(&root, 2);
	
	// insert(&root, 4);
	// insert(&root, 4);
	// insert(&root, 4);
	// insert(&root, 4);
	// insert(&root, 4);
	// insert(&root, 4);
	// insert(&root, 4);
	// insert(&root, 4);
	
	// insert(&root, 0);
	// insert(&root, 0);
	// insert(&root, 0);
	// insert(&root, 0);

	// insert(&root, 1);

	// insert(&root, 100);
	// insert(&root, 50);
	// insert(&root, 200);
	// insert(&root, 25);
	// insert(&root, 75);
	// insert(&root, 150);
	// insert(&root, 250);
	// insert(&root, 30);
	// insert(&root, 230);

	// delete(&root, 0, NULL, 0);
	// delete(&root, 1, NULL, 0);
	// delete(&root, 2, NULL, 0);
	// delete(&root, 1, NULL, 0);
	// delete(&root, 1, NULL, 0);

	// delete(&root, 0, NULL);
	// delete(&root, 1, NULL);
	// delete(&root, 2, NULL);
	// delete(&root, 1, NULL);
	// delete(&root, 1, NULL);

	// delete(&root, 1, NULL);
	// delete(&root, 1, NULL);
	// delete(&root, 2, NULL);


	// delete(&root, 75, NULL);

	// insert(&root, 20);
	// insert(&root, 10);
	// insert(&root, 100);
	// insert(&root, 5);

	// insert(&root, 11);
	// insert(&root, 99);
	// insert(&root, 105);
	// insert(&root, 4);
	// insert(&root, 98);
	// insert(&root, 104);
	// insert(&root, 1000);
	// insert(&root, 2000);


	// insert(&root, 90);
	// insert(&root, 110);
	// insert(&root, 80);
	// insert(&root, 120);
	// insert(&root, 91);
	// insert(&root, 3);
	// insert(&root, 8);
	// insert(&root, 37);
	// insert(&root, 60);
	// insert(&root,24);
	// insert(&root, 19);
	//print_preorder(root);
	// //printf("next\n");
	// delete(&root, 21, NULL);
	// delete(&root, 11, NULL);
	// delete(&root, 64365, NULL);
		 //delete(&root, 10, NULL);
	
	//printf("now\n");
	
	//root = tree_from_post(output);
	// printf("check:%d\n",root->left == NULL);
	
	return EXIT_SUCCESS;
}

