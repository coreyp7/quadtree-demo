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

void Tree::remove(int value) {
	// Value matches, delete this.
	if (this->value == value) {
		// remove this and fix the pointers to where they should be
		if (right != nullptr) {

			// replace pointers from this Tree to right's smallest child node.
			//right->min()->parent = this->parent;
			//if(right->min()->value != right->value){
			//	right->min()->right = this->right;
			//}
			//else {
			//	right->min()->right = nullptr;
			//}
			//// set this->left to be the new node's left.
			//if (left != nullptr) {
			//	right->min()->left = left;
			//}
			//
			//// Set parent's pointer from this to right's min node.
			//if (parent->value > this->value) {
			//	parent->left = right->min();
			//}
			//else if (parent->value < this->value) {
			//	parent->right = right->min();
			//}



		}
		else if (left != nullptr) {
			left->parent = this->parent;
			
			if (parent->value > this->value) {
				parent->left = left;
			}
			else if (parent->value < this->value) {
				parent->right = left;
			}
		}
		delete this;
	}
	else { // Go down the tree further to find the value.
		if (value < this->value) {
			left->remove(value);
		}
		else if (value > this->value) {
			right->remove(value);
		}
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
bool Tree::valid() {

	if (left != nullptr && right != nullptr) {
		if (!(left->value < right->value)) {
			return false;
		}
		else if (!(value > left->value || value < right->value)) {
			return false;
		}
		return left->valid() && right->valid();
	}
	// this is a leaf node, so just return true.
	return true;
}

Tree* Tree::min() {
	if (left == nullptr) {
		return this;
	}

	return left->min();
}

Tree* Tree::max() {
	if (right == nullptr) {
		return this;
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
