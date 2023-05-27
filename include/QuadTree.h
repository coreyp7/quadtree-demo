#pragma once
#include <SDL.h> // using only for certain structs
#include <vector>
#include "Dot.h"

class QuadTree {
public:

	static const int LIMIT = 2;

	// Pointers to all children of this 'node'. 
	QuadTree* nw;
	QuadTree* ne;
	QuadTree* sw;
	QuadTree* se;

	// Leaving for convenience to easily check if this is a leaf (and thus, all children references are nullptr).
	bool isLeaf;

	// TEMPORARY: will contain squares later. Getting it working with points first because its easier to think about.
	// Also, I haven't figured out if points which are in the children appear here; will determine during implementation.
	std::vector<Dot> points;

	// Position of this QuadTree square (from top left).
	int x, y, width, height;

	QuadTree(float x, float y, float width, float height);
	~QuadTree();

	void insert(Dot point);

	// For confirming if a point belongs in this QuadTree (square).
	bool insideOf(Dot point);

	// Will draw this entire QuadTree and all of its children.
	// (Will also draw the points).
	void draw(SDL_Renderer* renderer);
};