#include <stdio.h>
#include <stdlib.h>
#include "shell_array.h"


long *Array_Load_From_File(char *filename, int *size) {
	FILE *fp;
	long int len;
	long *array;

	fp = fopen(filename,"rb");
	if(fp == NULL) {
		*size = 0;
		return NULL; 
	}

	fseek(fp, 0, SEEK_END);
	len =  ftell(fp);
	array = malloc(len);
	if(array == NULL) {
		*size = 0;
		return NULL;
	}
	fseek(fp, 0 , SEEK_SET);

	fread(array, sizeof(long), len / sizeof(long), fp);
	fclose(fp);
	*size = len / sizeof(long);	
	// int i = 0;
	// while(array[i]) {
	// 	printf("real %ld \n", array[i]);
	// 	i++; }

	
	return array; 
}

int Array_Save_To_File(char *filename, long *array, int size) {
	FILE *fp;
	fp = fopen(filename, "wb+");
	if(fp == NULL) {
		return -1;
	}
	size_t num_written = 0;
	if(array != NULL) {
		num_written = fwrite(array, sizeof(long), size, fp);
	}
	fclose(fp);
	return num_written;

}
void Array_Shellsort(long *array, int size, long *n_comp) {
	int org; 
	int sub;
	long tmp;
	int interval = 1;
	if(array == NULL) {
		return;
	}

	while(interval < size / 3) {
		interval = interval*3 + 1; 
	}
	while(interval > 0) {
		for(org = interval; org < size; org++) {
			tmp = array[org];
			sub = org;
			*n_comp += 1;
			while((sub > interval - 1) && array[sub - interval] >= tmp) {
				*n_comp += 1;
				array[sub] = array[sub - interval];
				sub = sub - interval;
			}
			array[sub] = tmp;
		}
		interval = (interval - 1) / 3;
	}
}
