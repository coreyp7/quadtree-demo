#include "util.h"


void swap(int* item1, int* item2) {
	int temp = *item1;
	*item1 = *item2;
	*item2 = temp;
}

void printArray(int arr[], int size) {
	printf("[");
	for (int i = 0; i < size; i++) {
		printf("%i,", arr[i]);
	}
	printf("]\n");
}

bool isSorted(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		if (i < size - 1) {
			//assert(items[i] <= items[i + 1]);
			if (!(arr[i] <= arr[i + 1])) {
				return false;
			}
		}
	}
	return true;
}