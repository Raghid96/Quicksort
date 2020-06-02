#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_ITEMS 100000000
#define swap(v, a, b) {unsigned tmp; tmp=v[a]; v[a]=v[b]; v[b]=tmp;}
// Static declaration of the code
static int *array;

// Printing the vector
static void print() {
	int i;

	for (i = 0; i < MAX_ITEMS; i++) {
		printf("%d\n", array[i]);
	}
	printf("\n");
}

// Intitalize the vector
static void init() {
	int i;
	array = (int *) malloc(MAX_ITEMS*sizeof(int));
	for (i = 0; i < MAX_ITEMS; i++) {
		array[i] = rand();
	}
}

// Here the partition will sort the vector from smallest to highest element
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




// use partition to sort the vectors
static void quick_sort(int *array, unsigned low, unsigned high) {

	unsigned pivot;

	if (low >= high)
		return;
	pivot = (low+high)/2;
	pivot = partition(array, low, high, pivot);
	if (low < pivot)
		quick_sort(array, low, pivot-1);
	if (pivot < high)
		quick_sort(array, pivot+1, high);
}



// Main method
int main(int argc, char **argv) {
	// Intiate the vector
	init();
	// Sorting the vector
	quick_sort(array, 0, MAX_ITEMS-1);
	free(array);
}

