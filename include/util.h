#pragma once
#include <stdio.h>

/* Swap the values of these two integers.*/
void swap(int* item1, int* item2);

void printArray(int arr[], int size);

/* Just for validating that its actually sorted.
TODO: change to be an assertion.*/
bool isSorted(int arr[], int size);