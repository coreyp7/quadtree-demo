#include "sorting.h"

// Right now its just selection sort
void sort(int* items, int count) {
	int i;

	for (i = 0; i < count-1; i++) {

		int min = -1;
		int minIndex = -1;

		for (int j = i+1; j < count; j++) {
			if (items[j] < min || min == -1) {
				min = items[j];
				minIndex = j;
			}
		}

		// Swap i and minIndex
		int temp = items[i];
		items[i] = min;
		items[minIndex] = temp;
	}
	// array is now sorted
}