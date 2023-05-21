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
			left->parent = this;
		}
	}
	else if (newValue > value) {
		//insert right
		if (right != nullptr) {
			right->insert(newValue);
		}
		else {
			right = new Tree(newValue);
			right->parent = this;
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

// Returns whether or not this is a valid bst.
bool Tree::isBST() {

	if (left != nullptr && right != nullptr) {
		if (!(left->value < right->value)) {
			return false;
		}
		else if (!(value > left->value || value < right->value)) {
			return false;
		}
		return left->isBST() && right->isBST();
	}
	// this is a leaf node, so just return true.
	return true;
}

int Tree::min() {
	if (left == nullptr) {
		return value;
	}

	return left->min();
}

int Tree::max() {
	if (right == nullptr) {
		return value;
	}

	return right->max();
}

std::vector<int> Tree::getOrderedVector() {
	std::vector<int> nodes;
	// Add all the values less than this value to nodes.
	if (left != nullptr) {
		std::vector<int> leftTree = left->getOrderedVector();
		//nodes.insert(nodes.end(), leftTree.begin(), leftTree.end());
		std::copy(leftTree.begin(), leftTree.end(), std::back_inserter(nodes));
	}

	// Add this value to nodes
	nodes.push_back(value);

	// Add all the values less than this value to nodes.
	if (right != nullptr) {
		std::vector<int> rightTree = right->getOrderedVector();
		//nodes.insert(nodes.end(), rightTree.begin(), rightTree.end());
		std::copy(rightTree.begin(), rightTree.end(), std::back_inserter(nodes));
	}

	return nodes;
}

std::string Tree::to_string() {
	std::vector<int> bst = getOrderedVector();
	//printf("bst printed:[");
	std::string str;
	str.append("[");
	for (int i = 0; i < bst.size(); i++) {
		str.append(std::to_string(bst[i]) + ", ");
	}
	str.append("]");
	return str;
}

Tree::~Tree() {

}
