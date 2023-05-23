#include "QuadTree.h"

QuadTree::QuadTree(float x, float y, float width, float height) {
	nw = nullptr;
	ne = nullptr;
	sw = nullptr;
	se = nullptr;
	isLeaf = true;

	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;
}

QuadTree::~QuadTree() {

}

void QuadTree::insert(SDL_FPoint point) {
	points.push_back(point);

	// Are there too many points in this square?
	// If so, then create our 4 quadtrees and insert
	// all my points into my (appropriate) children.
	if (points.size() >= LIMIT) {
		isLeaf = false;
		nw = new QuadTree(0, 0, width / 2, height / 2);
		ne = new QuadTree(width / 2, 0, width / 2, height / 2);
		sw = new QuadTree(0, height / 2, width / 2, height / 2);
		se = new QuadTree(width / 2, height / 2, width / 2, height / 2);

		// For now, I'm going to add the point to every tree
		// which it technically belongs to. I believe this
		// is how this is supposed to work.
		if (nw->insideOf(point)) {
			nw->insert(point);
		}
		if (ne->insideOf(point)) {
			ne->insert(point);
		}
		if (sw->insideOf(point)) {
			sw->insert(point);
		}
		if (se->insideOf(point)) {
			se->insert(point);
		}
		// I'm not sure if I'm supposed to do this, but
		// am going with it for now because it makes
		// more intuitive sense to me. 
		// (aka leafs can only have points).
		points.clear();
	}
}

bool QuadTree::insideOf(SDL_FPoint point) {
	if ((point.x < (x + width) && point.x > x)
		&& (point.y < (y + height) && point.y > y)) {
		return true;
	}
	return false;
}

/*
* What's next?
* 
* Well, you can add a 'insert' function which adds a point to
* the points vector.
* 
* This function will check whether adding this point hits the
* number of dots limint in this QuadTree.
*	if it does, then create all 4 children quad trees, set isLeaf
*	to false, and insert all of the points in the points vector into
*	their appropriate quadtree quadrants.
* 
* So that we can see whats going on, create a function called
* 'render_tree(SDL_Renderer*)' so that we can easily/internally
* render what is actually going on.
* I think it'll be self explanatory: draw the box of the QuadTree,
* and if its not a leaf, then call the function in all of its 
* children, thus recursively calling it on everything.
* (also want to render the points that are in each quadrant).
*/