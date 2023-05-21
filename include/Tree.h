#pragma once
#include <vector>
#include <iostream>
#include <string>

class Tree {

public:
	int value;

	Tree* parent;
	Tree* left;
	Tree* right;

	// For root node
	Tree(int value);
	~Tree();

	void insert(int value);
	Tree* searchTree(int value);
	int min();
	int max();
	std::vector<int> getOrderedVector();
	std::string to_string();

	bool isBST();

};