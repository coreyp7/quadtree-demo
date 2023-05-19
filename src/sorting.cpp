#include "sorting.h"

// Right now its just selection sort
void sort(int* items, int count) {
	int i, minIndex;

	for (i = 0; i < count-1; i++) {

		minIndex = i;
		for (int j = i+1; j < count; j++) {
			if (items[j] < items[minIndex]) {
				minIndex = j;
			}
		}

		// Swap min value with the current position in array (i).
		if (minIndex != i) {
			swap(&items[i], &items[minIndex]);
		}
	}
	// array is now sorted
}