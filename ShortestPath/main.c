#include <stdio.h>
#include <stdlib.h>
#include "grid2.h"

int main(int argc, char* argv[]) {
    if(argc != 5) {
        return EXIT_FAILURE;
    }
    char* input = argv[1];
    char* output = argv[2];
    char* time_output = argv[3];
    char* path_output = argv[4];
    short* rows = malloc(sizeof(short));
    short* cols = malloc(sizeof(short));
    
    int* final_pred = NULL;
    *rows = 0;
    *cols = 0;
    int exit_ind = 0;

    short* times = NULL;
    times =  output_grid(input,output,rows,cols);
    if(times == NULL) {
        free(rows);
        free(cols);    
        return EXIT_FAILURE;   
    }
    //int size = (*rows) * (*cols);
    // for(int i = 0; i<size; i++) {
    //     printf("%hd\n",times[i]);
    // }
    int* fastest_paths = find_paths(times,*rows,*cols,&final_pred, &exit_ind);
    if(fastest_paths == NULL) {
        free(rows);
        free(cols);  
        free(times);
        
        return EXIT_FAILURE;       
    }
    if(final_pred == NULL) {
        free(rows);
        free(cols);  
        free(times);
        free(fastest_paths);
        return EXIT_FAILURE;        
    }
 //   printf("exit: %d\n",exit_ind);
    // for(int i = 0; i < *cols; i++) {
    //     printf("path %d: %d \n",i,fastest_paths[i]);
    // }
    // for(int i = 0; i < size; i++) {
    //     printf("pred[%d]: %d\n",i,final_pred[i]);
    // }

    write_times(fastest_paths, cols, time_output);
    write_paths(fastest_paths, final_pred, *cols, *rows, exit_ind, path_output);
    free(rows);
    free(cols);
    if(fastest_paths != NULL) {
        free(fastest_paths);
    }
    if(times != NULL) {
        free(times);
    }
    if(final_pred != NULL) {
        free(final_pred);
    }
    
    return EXIT_SUCCESS;

}