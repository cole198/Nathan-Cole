#include <stdbool.h>
typedef struct Nodes {
    int key;
    int path;
    
} Node;
int* find_paths(short* times, int rows, int cols, int** final_pred, int* exit_ind);
void write_times(int* paths, short* cols, char* output);
short* output_grid(char* input, char* output, short* rows, short* cols);

void write_paths(int* fastest_paths, int* final_pred, short cols, short rows, int exit_ind, char* output);
int get_min_heap(int* heap_loc, int heapsize);
void down_heap(Node* paths, int* loc, int index, int heapsize);
void up_heap(Node* paths, int* loc, int index);
void swap(Node* a, Node* b, int* loc);