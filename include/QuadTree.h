#pragma once
#include <SDL.h> // using only for certain structs
#include <vector>

class QuadTree {
public:

	// Pointers to all children of this 'node'. 
	QuadTree* nw;
	QuadTree* ne;
	QuadTree* sw;
	QuadTree* se;

	// Leaving for convenience to easily check if this is a leaf (and thus, all children references are nullptr).
	bool isLeaf;

	// TEMPORARY: will contain squares later. Getting it working with points first because its easier to think about.
	// Also, I haven't figured out if points which are in the children appear here; will determine during implementation.
	std::vector<SDL_Point> points;

	// Position of this QuadTree square (from top left).
	int x, y, width, height;

	QuadTree(int x, int y, int width, int height);
	~QuadTree();


};