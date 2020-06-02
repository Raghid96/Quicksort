#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define MAX_ITEMS 100000000
#define swap(v, a, b) {unsigned tmp; tmp=v[a]; v[a]=v[b]; v[b]=tmp;}
// 
static int counter = 0;
// Visible for all methods
static int *array;

// Printing the vector
static void print() {
	int i;
	for (i = 0; i < MAX_ITEMS; i++) {
		printf("%d\n", array[i]);
	}
	printf("\n");
}

// Initiate the vector with random values
static void init() {
	int i;
	array = (int *) malloc(MAX_ITEMS*sizeof(int));
	for (i = 0; i < MAX_ITEMS; i++) {
		array[i] = rand();
	}
}




// partition part
static unsigned partition(int *array, unsigned low, unsigned high, unsigned pivot_index) {
	if (pivot_index != low)
		swap(array, low, pivot_index);
	pivot_index = low;
	low++;
	while (low <= high) {
		if (array[low] <= array[pivot_index])
			low++;
		else if (array[high] > array[pivot_index])
			high--;
		else
			swap(array, low, high);
	}
	if (high != pivot_index)
		swap(array, pivot_index, high);
	return high;
}


// openMP part that sorts the elements with partition method
static void quick_sort(int *array, unsigned low, unsigned high) {
	unsigned pivot;

	if (low >= high)
		return;
	pivot = (low+high)/2;
	pivot = partition(array, low, high, pivot);
#pragma omp parallel sections num_threads(8)
	{
#pragma omp section
		{
			if (low < pivot) {
				quick_sort(array, low, pivot-1);
			}
		}
#pragma omp section
		{
			if (pivot < high) {
				quick_sort(array, pivot+1, high);
			}
		}
	}
}


// The main method
int main(int argc, char **argv) {
	// Initiate the vector
	init();
	// quicksort the vector using openMP
	quick_sort(array, 0, MAX_ITEMS-1);
	// Free the allocated memory for the vector. 
	free(array);
}
