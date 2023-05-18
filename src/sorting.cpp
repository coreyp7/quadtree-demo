#include "sorting.h"

// Right now its just selection sort
void sort(std::vector<int> items) {
	int index = 0;

	for (int i = 0; i < items.size(); i++) {
		int minIndex = -1;
		int minValue = -1;

		for (int j = 0; j < items.size(); j++) {
			if (items[j] < minValue || minValue == -1) {// this assumes that all items are positive
				minValue = items[j];
				minIndex = j;
			}
		}

		int iOld = items[i];
		items[i] = minValue;
		items[minIndex] = iOld;
	}
}