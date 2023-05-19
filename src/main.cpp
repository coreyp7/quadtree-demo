#include "sorting.h"
#include <cstdlib>
#include <cassert>

const int COUNT = 1000;

int main(int, char**){

	// Make random array
	int items[COUNT];
	for (int i = 0; i < COUNT; i++) {
		int numb = rand() % 1000;
		items[i] = numb;
	}

	printf("before:");
	printArray(items, COUNT);

	sort(items, COUNT);

	printf("after:");
	printArray(items, COUNT);
	printf("Status: %s", (isSorted(items, COUNT)) ? "sorted" : "NOT SORTED");
}