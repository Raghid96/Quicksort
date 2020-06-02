#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_ITEMS 100
#define MAX_THREADS 8
#define SET_SIZE (MAX_ITEMS/MAX_THREADS)
#define swap(v, a, b) {unsigned tmp; tmp=v[a]; v[a]=v[b]; v[b]=tmp;}

// v is the vector
static int *v;
// new thread with MAX_THREADS as number of threads
pthread_t tid[MAX_THREADS];

typedef struct {
	int end;
	int start;
} Args;


typedef struct {
	unsigned leftStart;
	unsigned rightStart;
	unsigned setSize;
} MergeArgs;


// Printing the vector
static void print_array(void) {
	int i;
	for(i = 0; i < MAX_ITEMS; i++) {
		printf("%d\n", v[i]);
	}
	printf("\n");
}


// Intiate the vector as before
static void init_array(void) {
	int i;
	v = (int *) malloc (MAX_ITEMS*sizeof(int));
	for (i = 0; i < MAX_ITEMS; i++) {
		v[i] = rand();
	}
}




// Partitions
static unsigned partition(int *v, unsigned low, unsigned high, unsigned pivot_index) {
	if (pivot_index != low)
		swap(v, low, pivot_index);
	pivot_index = low;
	low++;
	while (low <= high) {
		if (v[low] <= v[pivot_index])
			low++;
		else if (v[high] > v[pivot_index])
			high--;
		else
			swap(v, low, high);
	}
	if (high != pivot_index)
		swap(v, pivot_index, high);
	return high;
}



// quicks sort with 
static void quick_sort(int *v, unsigned low, unsigned high) {
	unsigned pivot_index;
	if (low >= high)
		return;
	pivot_index = (low+high)/2;
	pivot_index = partition(v, low, high, pivot_index);
	if (low < pivot_index)
		quick_sort(v, low, pivot_index-1);
	if (pivot_index < high)
		quick_sort(v, pivot_index+1, high);
}






void * sortWorker(void * arg) {
	Args args = *((Args *) arg);
	quick_sort(v, args.start, args.end-1);
	free(arg);
	pthread_exit(0);
}






void * mergeWorker(void * arg) {
	MergeArgs args = *((MergeArgs *) arg);
	int * left = v + args.leftStart;
	int * right = v + args.rightStart;
	int * temp = malloc((args.setSize * 2)*sizeof(int));
	int size = args.setSize;
	int i,j,k;
	i = j = k = 0;
	while(i < size && j < size) {
		if(left[i] <= right[j]) {
			temp[k++] = left[i++];
		}
		else {
			temp[k++] = right[j++];
		}
	}
	while(i < size)
		temp[k++] = left[i++];
	while(j < size)
		temp[k++] = right[j++];
	i = 0;
	int s = args.leftStart;
	for(; i < args.setSize * 2; i++) {
		v[s++] = temp[i];
	}
	free(arg);
	free(temp);
	pthread_exit(0);
}






int main(int argc, char **argv) {

	init_array();
	print_array();
	int i = 0;
	// Use Pthread to compute the sorting
	for(; i < MAX_THREADS; i++) {
		Args * args = malloc(sizeof(* args));
		args->start = SET_SIZE * (i+1) - SET_SIZE;
		args->end = SET_SIZE * (i+1);
		pthread_create(&tid[i], 0, sortWorker, args);
	}
	// Joining the threads
	i = 0;
	for(; i < MAX_THREADS; i++) {
		pthread_join(tid[i], NULL);
	}
	
	
	i = 0;
	for(; i < MAX_THREADS/2; i++) {
		if(i == 0) {
			MergeArgs * args = malloc(sizeof(* args));
			args->leftStart = 0;
			args->rightStart = SET_SIZE;
			args->setSize = SET_SIZE;
			pthread_create(&tid[i], 0, mergeWorker, args);
		}
		else if(i == 1) {
			MergeArgs * args = malloc(sizeof(* args));
			args->leftStart = SET_SIZE * 2;
			args->rightStart = SET_SIZE * 3;
			args->setSize = SET_SIZE;
			pthread_create(&tid[i], 0, mergeWorker, args);
		}
		else if(i == 2) {
			MergeArgs * args = malloc(sizeof(* args));
			args->leftStart = SET_SIZE * 4;
			args->rightStart = SET_SIZE * 5;
			args->setSize = SET_SIZE;
			pthread_create(&tid[i], 0, mergeWorker, args);
		}
		else {
			MergeArgs * args = malloc(sizeof(* args));
			args->leftStart = SET_SIZE * 6;
			args->rightStart = SET_SIZE * 7;
			args->setSize = SET_SIZE;
			pthread_create(&tid[i], 0, mergeWorker, args);
		}
	}
	i = 0;
	for(; i < MAX_THREADS/4; i++) {
		pthread_join(tid[i], NULL);
	}
	i = 0;
	for(; i < MAX_THREADS/4; i++) {
		if(i==0) {
			MergeArgs * args = malloc(sizeof(* args));
			args->leftStart = 0;
			args->rightStart = SET_SIZE * 2;
			args->setSize = SET_SIZE * 2;
			pthread_create(&tid[i], 0, mergeWorker, args);
		}
		else {
			MergeArgs * args = malloc(sizeof(* args));
			args->leftStart = SET_SIZE * 4;
			args->rightStart = SET_SIZE * 6;
			args->setSize = SET_SIZE * 2;
			pthread_create(&tid[i], 0, mergeWorker, args);
		}
	}
	i = 0;
	for(; i < MAX_THREADS/4; i++) {
		pthread_join(tid[i], NULL);
	}
	i = 0;
	MergeArgs * args = malloc(sizeof(* args));
	args->leftStart = 0;
	args->rightStart = SET_SIZE * 4;
	args->setSize = SET_SIZE * 4;
	pthread_create(&tid[0], 0, mergeWorker, args);
	pthread_join(tid[0], NULL);
	print_array();

}
