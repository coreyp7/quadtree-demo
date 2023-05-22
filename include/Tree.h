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
	void remove(int value);
	Tree* searchTree(int value);
	Tree* min();
	Tree* max();
	std::vector<int> getOrderedVector();
	std::string to_string();

	bool valid();

};