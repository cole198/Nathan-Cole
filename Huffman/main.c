#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char* argv[]) {
	if(argc != 7) {
		return EXIT_FAILURE;
	}
	char* coded_input = argv[1];
	char* tree_out = argv[2];
	char* decoded = argv[3];
	char* freqs = argv[4];
	char* huff = argv[5];
	char* eval = argv[6];
	FILE* fp = fopen(coded_input, "rb");
	if(fp == NULL) {
		return EXIT_FAILURE;
	}
	FILE* dec_fp = fopen(decoded, "wb+");
	FILE* freq_fp = fopen(freqs, "wb+");
	FILE* huff_fp = fopen(huff, "wb+");
	FILE* eval_fp= fopen(eval, "wb+");
	
	if(!fp || !dec_fp || !freq_fp || !huff_fp || !eval_fp) {
		return EXIT_FAILURE;
	}
	long com_bytes = 0;
	
	fread(&com_bytes, sizeof(long), 1, fp);
	
	long top_bytes = 0;
	fread(&top_bytes, sizeof(long), 1, fp);
	long uncom_bytes = 0;
	fread(&uncom_bytes, sizeof(long), 1,fp);
	// printf("uncom:%ld\n",uncom_bytes);
 	unsigned char* array = malloc(sizeof(char) * top_bytes);
	fread(array, sizeof(char), top_bytes,fp);
	unsigned char* encoded = malloc(sizeof(char) * (com_bytes - 24 - top_bytes));
	fread(encoded, sizeof(char), (com_bytes - 24 - top_bytes), fp);
	print_topology(array, top_bytes, tree_out); 
	TreeNode* Node = create_tree(array, top_bytes);
	long* freq;
	long non_huff_bits = 0;
	long huff_bits = 0;
	freq = decode(Node, encoded, com_bytes,uncom_bytes, dec_fp, &non_huff_bits);
	
	TreeNode* huff_pq =  make_huff_pq(freq);
	TreeNode* root = create_huff_tree(huff_pq);
//	topology_from_tree(root, huff_fp);
	decode(root, encoded, com_bytes, uncom_bytes, NULL, &huff_bits);
	topology_from_tree(root, huff_fp);
	// printf("huff: %ld, nonhuff: %ld", huff_bits, non_huff_bits);
	fwrite(freq, sizeof(long), 256, freq_fp);
	long non_huff_bytes = non_huff_bits / 8;
	long huff_bytes = huff_bits / 8;
	non_huff_bits = (int) (non_huff_bits % 8);
	huff_bits = (int) (huff_bits % 8);
	fwrite(&non_huff_bytes, sizeof(long), 1, eval_fp);
	fwrite(&(non_huff_bits), sizeof(int), 1, eval_fp);
	fwrite(&huff_bytes, sizeof(long), 1, eval_fp);
	fwrite(&(huff_bits), sizeof(int), 1, eval_fp);

	free(array);
	free(encoded);
	free(freq);
	destroy_tree(Node);
	destroy_tree(root);
	fclose(freq_fp);
	fclose(fp);
	fclose(dec_fp);
	fclose(huff_fp);
	fclose(eval_fp);

	return EXIT_SUCCESS;
}
