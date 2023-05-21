#include "Tree.h"

Tree::Tree(int value) {
	this->value = value;
	parent = nullptr;
	left = nullptr;
	right = nullptr;
}

void Tree::insert(int newValue) {
	if (newValue < value) {
		//insertleft
		if (left != nullptr) {
			left->insert(newValue);
		}
		else {
			left = new Tree(newValue);
		}
	}
	else if (newValue > value) {
		//insert right
		if (right != nullptr) {
			right->insert(newValue);
		}
		else {
			right = new Tree(newValue);
		}
	}
	else {
		//TODO
		// same value, idk pick one side
		// could disallow duplicates
	}
}

Tree* Tree::searchTree(int value) {
	//TODO: implement with a while loop, wikipedia says that it
	// performs more efficient (on "most machines").
	
	//TODO: change to check isLeaf field instead of nullptr stuff.
	if (value < this->value) {
		if (left != nullptr) {
			return left->searchTree(value);
		}
		else {
			return nullptr;
		}
	}
	else if (value > this->value) {
		if (right != nullptr) {
			return right->searchTree(value);
		}
	}
	else {
		return this;
	}
}

Tree::~Tree() {

}
