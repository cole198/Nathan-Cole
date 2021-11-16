	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "shell_array.h"
	#include "shell_list.h"


int main(int argc, char* argv[]) {
	
	if(argc != 4) {
		return EXIT_FAILURE;
	}
	char *input = argv[2];
	char *output = argv[3];
	long n_comp = 0;
	
	if(!strcmp(argv[1], "-a")) {
		long *array;
		int size;
		array = Array_Load_From_File(input, &size);
		if(array == NULL) {
			return EXIT_FAILURE;
		}
		
		Array_Shellsort(array, size, &n_comp);
		int x = Array_Save_To_File(output, array, size);
		// for(int i = 0; i<size; i++) {
		// 	printf("array %d: %ld\n",i,array[i]);
		// }

		free(array);
		printf("Number of Comparisons: %ld\n", n_comp);
		if(x == -1) {
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	else if(!strcmp(argv[1], "-l")) {
		Node* list;
		list = List_Load_From_File(input);
		list = List_Shellsort(list, &n_comp);
		int x = List_Save_To_File(output, list);
		printf("Number of Comparisons: %ld\n", n_comp);
		//printf("\nstarting\n");
		while(list != NULL) {
			Node* tmp = list;
			//printf("%ld\n",list->value);
			list = list->next;
			free(tmp);
		}
		//printf("end\n");
//		Node *tmp;
		// while(list != NULL) {
		// 	Node* tmp = list;
		// 	list = list->next;
		// 	free(tmp);
			
		// }	
		if(x == -1) {
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}	
//	Array_Save_To_File(output, array, size);
//
///	Node* list;
  //	list = List_Load_From_File(input);
//	list = List_Shellsort(list, &n_comp);
//	List_Save_To_File(output, list);
//	printf("%ld",n_comp);
//	Node *tmp;
//	while(list != NULL) {
//		tmp = list->next;
//		free(list);
//		list = tmp;
//	}

	return EXIT_FAILURE;
}
