// ...existing code...
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 1000000 // Max array size for testing

// Function to merge two subarrays
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

// Sequential Merge Sort
void sequential_mergesort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        sequential_mergesort(arr, l, m);
        sequential_mergesort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort using OpenMP sections
void parallel_mergesort(int arr[], int l, int r, int depth) {
    if (l < r) {
        int m = (l + r) / 2;
        if (depth <= 0) {
            // Fall back to sequential if max depth reached
            sequential_mergesort(arr, l, r);
        } else {
#pragma omp parallel sections
            {
#pragma omp section
                parallel_mergesort(arr, l, m, depth - 1);
#pragma omp section
                parallel_mergesort(arr, m + 1, r, depth - 1);
            }
            merge(arr, l, m, r);
        }
    }
}

void fill_array(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;
}

void copy_array(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

int main() {
    int n;
    printf("Enter number of elements (up to %d): ", MAX);
    scanf("%d", &n);

    if (n > MAX) {
        printf("Array size too large!\n");
        return 1;
    }

    int *arr_seq = (int *)malloc(n * sizeof(int));
    int *arr_par = (int *)malloc(n * sizeof(int));

    fill_array(arr_seq, n);
    copy_array(arr_seq, arr_par, n);

    // Sequential mergesort timing
    double start_seq = omp_get_wtime();
    sequential_mergesort(arr_seq, 0, n - 1);
    double end_seq = omp_get_wtime();

    // Parallel mergesort timing
    double start_par = omp_get_wtime();
    parallel_mergesort(arr_par, 0, n - 1, 4); // depth = 4 gives 16 tasks max
    double end_par = omp_get_wtime();

    printf("\nTime taken by Sequential MergeSort: %.6f seconds\n", end_seq - start_seq);
    printf("Time taken by Parallel MergeSort : %.6f seconds\n", end_par - start_par);

    free(arr_seq);
    free(arr_par);

    return 0;
}


// steps to run gcc -fopenmp parallel_mergesort.c -o mergesort
//./mergesort
