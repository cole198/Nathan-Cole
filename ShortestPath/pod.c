#include <stdio.h>
#include <stdlib.h>
#include "grid2.h"
#include <limits.h>
#include <stdbool.h>
//static int extract_min(int* paths, bool* visited, int size);
static int get_min(int* fastest_paths, short cols);

// static int extract_min(int* paths, bool* visited, int size) {
//     if(paths == NULL) {
//         return 0;
//     }
//     int min = INT_MAX;
//     int index = 0;
//     int count = 0;
//     for(int i = 0; i < size; i++) {
//         count += 2;
//         if((paths[i] < min) && !(visited[i])) {
            
//             min = paths[i];
//             index = i;
//         }
//     }
//     //printf("comp: %d\n",count);
//     return index;
// }
static int get_min(int* fastest_paths, short cols) {
    int fastest = INT_MAX;
    int index = 0;
    for(int i = 0; i < cols; i++) {
        if(fastest > fastest_paths[i]) {
            index = i;
            fastest = fastest_paths[i];
        }
    }
    return index;
}

short* output_grid(char* input, char* output, short* rows, short* cols) {
    FILE* fp_in = fopen(input, "rb");
    if(fp_in == NULL) {
        return NULL;
    }
    FILE* fp_out = fopen(output, "wb+");
    if(fp_out == NULL) {
        fclose(fp_in);
        return NULL;
    }
    // int rows = 0;
    // int cols = 0;
    fread(rows, sizeof(short), 1, fp_in);
    fread(cols, sizeof(short), 1, fp_in);
    
    //printf("rows: %d cols: %d\n",*rows,*cols);

    fprintf(fp_out, "%hd %hd\n",*rows,*cols);
    int x = 0;
    int size = (*rows)*(*cols);
    //printf("sizeL %d\n",size);
    //int count = 0;
    // int times[rows*cols][2];
    short* times;
    times = malloc(sizeof(short)*size + sizeof(short));
    times[size] = '\0';
    for(int i = 0; i < *rows; i++) {
        for(int j = 0; j < *cols; j++) {
            fread(&x, sizeof(short), 1, fp_in);
            fprintf(fp_out, "%hd", x);
            //printf("%hd\n",x);
            times[i*(*cols) + j] = x;
            //times[count][1] = i*cols + j;
            if(j != *cols - 1) {
                fprintf(fp_out, " ");
            }
            else{
                fprintf(fp_out, "\n");
            }
        }
    }
    fclose(fp_in);
    fclose(fp_out);
    return times;
    
}
void swap(Node* a, Node* b, int* loc) {
    int key = (*b).key;
    int path = (*b).path;
    int location = loc[(*b).key];
    loc[(*b).key] = loc[(*a).key];
    (*b).key = (*a).key;
    (*b).path = (*a).path;
    loc[(*a).key] = location;
    (*a).key = key;
    (*a).path = path;
    
}

void up_heap(Node* paths, int* loc, int index) {
    int temp = paths[index].path;
    //int i = min_heap[index];
    int child = index;
    int parent = (child-1)/2;
    while(child > 0 && paths[parent].path > temp) {
        swap(&paths[child],&paths[parent], loc);
        child = parent;
        parent = (child-1)/2;
    }
    //min_heap[child] = index;
    return;
}

void down_heap(Node* paths, int* loc, int index, int heapsize) {
    int temp = paths[index].path;
   // int key = paths[index].key;
    //int loca = loc[paths[index].key];
    //printf("temp %d key:%d loc: %d\n",temp,key,loca);
    //int og = min_heap[index];
    int j = 2*index+1;
    while(j <= heapsize - 1) {
        if((j < heapsize - 1) && paths[j].path > paths[j+1].path) {
            j++;

        }
        if(temp <= paths[j].path) {
            //printf("nah\n");
            break;
        }
        else {
            //printf("ye\n");
            //min_heap[index] = min_heap[j];
            //printf("swap key: %d index key: %d\n",paths[j].key, loc[paths[j].key]);

            // int temp1 = loc[paths[index].key];
            // loc[paths[index].key] = loc[paths[j].key];
            // loc[paths[j].key] = temp1;
            // paths[index] = paths[j];
            swap(&paths[index], &paths[j], loc);
            // loc[paths[index].key] = loca;
            // paths[index].key = key;
            // paths[index].path = temp;
            index = j;
        }
        j = 2*index+1;
    }
    // loc[paths[index].key] = loca;
    // paths[index].key = key;
    // paths[index].path = temp;
    

}
int* find_paths(short* times, int rows, int cols, int** final_pred, int* exit_ind) {
    if(times == NULL) {
        return NULL;
    }
    Node* paths = malloc((sizeof(Node) * rows*cols) + sizeof(Node));
    bool* visited = malloc((sizeof(bool) * rows * cols) + sizeof(bool));
    int* fastest_paths = malloc((sizeof(int)*cols) + sizeof(int));
    *final_pred = malloc(sizeof(int)*cols*rows + sizeof(int));
    int* pred = malloc(sizeof(int)*cols*rows + sizeof(int));
    int* loc = malloc(sizeof(int)*cols*rows);
    int heapsize = 0;
    // final_pred[rows*cols] = '\0';
    // pred[rows*cols] = '\0';
    // fastest_paths[cols] = '\0';
    // visited[rows*cols] = '\0';
    // paths[rows*cols] = '\0';

    for(int start = 0; start < cols; start++) {
        for(int i = 0; i<rows*cols; i++) {
        paths[i].path = INT_MAX;
        paths[i].key = i;
        loc[i] = i;
        visited[i] = 0;
        pred[i] = -1;
        heapsize++;
        }
    //faijfhaoi
        paths[start].path = 0; 
        swap(&paths[0],&paths[start], loc);

        int i = 0;
        while(i < rows*cols) {
            int index = paths[0].key;
            swap(&paths[0], &paths[heapsize-1], loc);
            heapsize--;
            down_heap(paths, loc, 0, heapsize);
            visited[index] = 1;
            int upper = index - cols;
            int lower = index + cols;
            bool left_chk = (index - 1 >= 0) && ((index) % cols != 0);
            bool right_chk = (index + 1 < rows*cols) && ((index+1) % cols != 0);
            int left = index - 1;
            int right = index + 1;
            if(upper >= 0) { //upper neighbor
                if(((!visited[upper]) && paths[loc[upper]].path > paths[loc[index]].path + times[index]) ) {
                    paths[loc[upper]].path = paths[loc[index]].path + times[index];
                    pred[upper] = index;
                    up_heap(paths, loc, loc[upper]);
                }
            }
            if(lower < rows*cols) { //lower neighbor
                if(((!visited[lower]) && paths[loc[lower]].path > paths[loc[index]].path + times[index]) ) {
                    paths[loc[lower]].path = paths[loc[index]].path + times[index];
                    pred[lower] = index;
                    up_heap(paths, loc, loc[lower]);
                }
            }   
            if(left_chk) {
                if(((!visited[left]) && paths[loc[left]].path > paths[loc[index]].path + times[index]) ) {
                    paths[loc[left]].path = paths[loc[index]].path + times[index];
                    pred[left] = index;
                    up_heap(paths, loc, loc[left]);
                }
            }  
            if(right_chk) {
                if(((!visited[right]) && paths[loc[right]].path > paths[loc[index]].path + times[index]) ) {
                    paths[loc[right]].path = paths[loc[index]].path + times[index];
                    pred[right] = index;
                    up_heap(paths, loc, loc[right]);
                }
            }           
            
            i++;
        }
        int final_ind = 0;
        int fast_path = INT_MAX;
        fastest_paths[start] = fast_path;
        for(int k = (rows-1)*cols; k < rows*cols; k++) {
            if(paths[loc[k]].path + times[k] < fast_path) {
                fastest_paths[start] = paths[loc[k]].path + times[k];
                fast_path = fastest_paths[start];
                final_ind = k;
            }
        }
        bool fastest = 1;

        for(int k = start; k >= 0; k--) {
            if(fastest && (fastest_paths[start] > fastest_paths[k])) {
                fastest = 0;
            }
        }
        if(fastest || start == 0) {
            *exit_ind = final_ind;
            //printf("yeayeyaye start: %d\n",start);
            //printf
            for(int k = 0; k < rows*cols; k++) {

                (*final_pred)[k] = pred[k];
            }
            //final_pred = pred;
        }
        //fastest_paths[start] = paths[start];

    }
    
    free(visited);
    free(paths);
    free(pred);
    free(loc);
    return fastest_paths;



}



void write_times(int* paths, short* cols, char* output) {
    if(paths == NULL) {
        return;
    }
    FILE* fp = fopen(output, "wb+");
    if(fp == NULL) {
        return;
    }

    fwrite(cols, sizeof(short), 1, fp);
    fwrite(paths, sizeof(int), *cols, fp);
    fclose(fp);

}

void write_paths(int* fastest_paths, int* final_pred, short cols,short rows, int exit_ind, char* output) {
    if(fastest_paths == NULL) {
        return;
    }
    FILE* fp = fopen(output, "wb+");
    if(fp == NULL) {
        return;
    }
    int index = get_min(fastest_paths, cols);
    fwrite(&(fastest_paths[index]), sizeof(int), 1, fp);
    int locations = 1;
    int i = exit_ind;
    while(final_pred[i] != -1) {
        locations++;
        i = final_pred[i];
    }
    //printf("loc:%d\n",locations);
    fwrite(&locations, sizeof(int), 1, fp);
    int* temp = malloc(sizeof(int) * locations);
    i = exit_ind;
    temp[locations-1] = exit_ind;
    temp[0] = index;
    for(int k = locations - 1; k > 0; k --) {
        temp[k-1] = final_pred[i];
        i = final_pred[i];
        
        
    }
    for(int k = 0; k < locations; k++) {
        int row = temp[k] / cols;
        int col = temp[k] % cols;
        fwrite(&row, sizeof(short), 1, fp);
        fwrite(&col, sizeof(short), 1, fp);
        // printf("(%hd,%hd)\n",row,col);
        //printf("path %d: (%d,%d)\n",k,row,col);
    }

    free(temp);
    fclose(fp);
}

