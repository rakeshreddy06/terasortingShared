#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>


void write_output_file(char **keys, int actual_keys,const char *outputFile) ;
void mergeSort(char **arr, int l, int r);
void merge(char **arr, int l, int m, int r);

void read_file(const char *filename, char ***keys, int *num_keys) ;
void validate_sort(const char *output_filename);
unsigned long long getFileSize(const char *filename);

#define KEY_SIZE 100



typedef struct {
    char **arr;
    int l;
    int r;
} ThreadArgs;


unsigned long long getFileSize(const char *filename) {
    struct stat st;
    if(stat(filename, &st) != 0) {
        return -1; // Error condition, you should handle this in your code.
    }
    return st.st_size;
}

// threading for merge sort
void* threadedMergeSort(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    mergeSort(args->arr, args->l, args->r);
    pthread_exit(0);
}


//parallel merging using threads
void parallelMergeSort(char **arr, int l, int r, int max_threads) {
    if (max_threads <= 1) {
        mergeSort(arr, l, r);
    } else {
        int mid = l + (r - l) / 2;

        pthread_t left_thread;
        pthread_t right_thread;

        ThreadArgs left_args = { arr, l, mid };
        ThreadArgs right_args = { arr, mid + 1, r };

        pthread_create(&left_thread, NULL, threadedMergeSort, &left_args);
        pthread_create(&right_thread, NULL, threadedMergeSort, &right_args);

        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        merge(arr, l, mid, r);
    }
}

//valsort operation 
void validate_sort(const char *output_filename) {
    char command[200];
    snprintf(command, sizeof(command), "./valsort %s", output_filename);
    system(command);
}


// read file of any input
void read_file(const char *filename, char ***keys, int *num_keys) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    } else {
        printf("Successfully opened file: %s\n", filename);
    }

    int i = 0;
    int capacity = 10;
    *keys = malloc(capacity * sizeof(char*));
    for (int j = 0; j < capacity; j++) {
        (*keys)[j] = malloc(KEY_SIZE * sizeof(char));
    }

    while (fgets((*keys)[i], KEY_SIZE, file) != NULL) {
        if (strcmp((*keys)[i], "\n") == 0 || strcmp((*keys)[i], "\r\n") == 0) {
    continue;
}

        
        strtok((*keys)[i], "\n");
        i++;

        if (i >= capacity) {
    int new_capacity = capacity * 2;
    
    *keys = realloc(*keys, new_capacity * sizeof(char*));
    for (int j = i; j < new_capacity; j++) {
        (*keys)[j] = malloc(KEY_SIZE * sizeof(char));
        (*keys)[j][0] = '\0';  // Initialize the new strings to empty
    }
    capacity = new_capacity;  // Update the capacity
}

    }

    *num_keys = i;
  
    fclose(file);
}

//general merge sort
void merge(char **arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // create temp arrays
    char **L = (char **)malloc(n1 * sizeof(char *));
    char **R = (char **)malloc(n2 * sizeof(char *));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

   
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // free the allocated memory for L and R
    free(L);
    free(R);
}

void mergeSort(char **arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}


void write_output_file(char **keys, int actual_keys,const char *output_file) {
    FILE *outputFile = fopen(output_file, "w");
    if (outputFile == NULL) {
        printf("Failed to open output file.\n");
        exit(1);
    }
    for (int i = 0; i < actual_keys; i++) {
        fprintf(outputFile, "%s\n", keys[i]);
    }
    fclose(outputFile);
}


//external sort using multithreading
int external_sort(const char *filename, int chunk_size) {
    FILE *inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        printf("Failed to open input file.\n");
        exit(1);
    }

    char **chunk = malloc(chunk_size * sizeof(char *));
    for (int i = 0; i < chunk_size; ++i) {
        chunk[i] = malloc(KEY_SIZE * sizeof(char));
    }

    int index = 0;
    int actual_size;  
    char buffer[KEY_SIZE];  

    while (1) {
        actual_size = 0;

        for (int i = 0; i < chunk_size; ++i) {
            if (fgets(buffer, KEY_SIZE, inputFile) != NULL) {


                strtok(buffer, "\n");  

                if (strcmp(buffer, "\n") != 0) {
                    strncpy(chunk[actual_size], buffer, KEY_SIZE);
                    actual_size++;
                }
            } else {
                break;  
            }
        }

        if (actual_size == 0) {
            break;  
        }

        mergeSort(chunk, 0, actual_size - 1);

        char temp_filename[50];
        sprintf(temp_filename, "temp_%d.txt", index);
        FILE *tempFile = fopen(temp_filename, "w");
        if (tempFile == NULL) {
            printf("Failed to open temporary file.\n");
            exit(1);
        }

        for (int i = 0; i < actual_size; ++i) {
            fputs(chunk[i], tempFile);
            fputs("\n", tempFile);
        }

        fclose(tempFile);
        ++index;
    }

    for (int i = 0; i < chunk_size; ++i) {
        free(chunk[i]);
    }
    free(chunk);
    fclose(inputFile);

    return index;
}

typedef struct {
    char key[KEY_SIZE];
    int file_index;
} HeapNode;

// Comparator function for min-heap
int compareHeapNode(const void *a, const void *b) {
    return strcmp(((HeapNode *)a)->key, ((HeapNode *)b)->key);
}


// this function have detailed comments for understanding
void merge_temp_files(int num_temp_files,const char *output_file) {
    FILE **tempFiles = malloc(num_temp_files * sizeof(FILE *));
    HeapNode *minHeap = malloc(num_temp_files * sizeof(HeapNode));
    int heapSize = 0;

    // Open all temporary files and populate initial heap
    for (int i = 0; i < num_temp_files; i++) {
        char temp_filename[50];
        sprintf(temp_filename, "temp_%d.txt", i);
        tempFiles[i] = fopen(temp_filename, "r");
        if (fgets(minHeap[heapSize].key, KEY_SIZE, tempFiles[i]) != NULL) {
            strtok(minHeap[heapSize].key, "\n");
            minHeap[heapSize].file_index = i;
            heapSize++;
        }
    }

    
    FILE *outputFile = fopen(output_file, "w");


    
    while (heapSize > 0) {
        // Pop the smallest key from heap
        qsort(minHeap, heapSize, sizeof(HeapNode), compareHeapNode);
        HeapNode smallest = minHeap[0];
        fprintf(outputFile, "%s\n", smallest.key);

        // Read next key from the same file to heap
        if (fgets(minHeap[0].key, KEY_SIZE, tempFiles[smallest.file_index]) != NULL) {
            strtok(minHeap[0].key, "\n");
        } else {
            // If file is empty, replace smallest with the last key in heap
            minHeap[0] = minHeap[heapSize - 1];
            heapSize--;
        }
    }

    
    for (int i = 0; i < num_temp_files; i++) {
        fclose(tempFiles[i]);
    }
    fclose(outputFile);
    free(tempFiles);
    free(minHeap);
  
}



void remove_empty_lines(const char *filename) {
  
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    }

   
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Failed to open temporary file.\n");
        fclose(file);
        exit(1);
    }

    char line[256]; 

 
    while (fgets(line, sizeof(line), file) != NULL) {
       
        line[strcspn(line, "\n")] = 0;
       
        if (strcmp(line, "") != 0) {
          
            fprintf(tempFile, "%s\n", line);
        }
    }

   
    fclose(file);
    fclose(tempFile);

    
    if (remove(filename) != 0) {
        printf("Failed to remove original file.\n");
        exit(1);
    }
    if (rename("temp.txt", filename) != 0) {
        printf("Failed to rename temporary file.\n");
        exit(1);
    }
}





int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please provide a filename as an argument.\n");
        return 1;
    }
    const char *filename = argv[1];
     size_t len1 = strlen(filename);
    size_t len2 = strlen(".log");
    char output_file[len1 + len2 + 1];  
    
  
    strcpy(output_file, filename);
    
   
    strcat(output_file, ".log");
    
    
    
   
    unsigned long long fileSize = getFileSize(filename);
      int max_threads = 8;
    char **keys = NULL;  
    int actual_keys = 0;  // To keep track of the actual number of keys read from the file 
   
    if (fileSize < 8589934592ULL) {  // less than 8GB
     clock_t start_time = clock();
   
        read_file(filename, &keys, &actual_keys);  // Read file and dynamically allocate keys array
        
         // Number of threads, 
        parallelMergeSort(keys, 0, actual_keys - 1, max_threads);  // Perform the sorting
          clock_t end_time = clock();
// Calculate time taken in milliseconds
    double time_taken = ((double)(end_time - start_time) / CLOCKS_PER_SEC ) * 1000;
    
    printf("Time taken to sort : %f milliseconds\n", time_taken);

    double throughput = fileSize / time_taken;

    printf("Throughput: %f  per second\n", throughput);
         write_output_file(keys, actual_keys,output_file);
        
        // validate sort and log
        validate_sort(output_file);
   
    } else {
        printf("opened external-sorting  this can time for sorting, writing into a file and validating \n");
        clock_t start_time = clock();
       
        // external sorting
        int index= external_sort(filename,10000000);
        merge_temp_files(index,output_file);
        for (int i = 0; i < index; i++) {
    char temp_filename[50];
    sprintf(temp_filename, "temp_%d.txt", i);
    remove(temp_filename);

}
  clock_t end_time = clock();
// Calculate time taken in milliseconds
    double time_taken = ((double)(end_time - start_time) / CLOCKS_PER_SEC ) * 1000;
    
    printf("Time taken to sort: %f milliseconds\n", time_taken);

    double throughput = fileSize / time_taken;

    printf("Throughput: %f  per second\n", throughput);
     printf("wait for valsort output");
     remove_empty_lines(output_file); 
          

        validate_sort(output_file);

 }

    //  free the dynamically allocated memory
    for (int i = 0; i < actual_keys; i++) {
        free(keys[i]);
    }
    free(keys);
    

    return 0;
}
