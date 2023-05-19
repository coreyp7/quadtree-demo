#include "sorting.h"

const int COUNT = 100;

int main(int, char**){

	int items[COUNT];
	for (int i = 0; i < COUNT; i++) {
		int numb = rand() % 100;
		items[i] = numb;
		printf("%i,", numb);
	}

	sort(items, COUNT);

	printf("\n");
	printf("sorted:");

	for (int i = 0; i < COUNT; i++) {
		printf("%i,", items[i]);
	}
	printf("\n");
}