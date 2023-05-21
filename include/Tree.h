#pragma once

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

	bool isBST();

};