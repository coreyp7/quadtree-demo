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
	if (!isLeaf) {
		delete nw;
		delete ne;
		delete sw;
		delete se;
	}
}

void QuadTree::insert(Dot* point) {

	// First check if this isn't a leaf node.
	if (!isLeaf) {
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
	}
	else {
		points.push_back(point);

		// Are there too many points in this leaf (square)?
		// If so, then create our 4 quadtrees and insert
		// all my points into my (appropriate) children.
		if (points.size() >= LIMIT) {
      if(depth >= DEPTH_LIMIT){
        return; 
        // ignore; stop making new quadtrees to avoid stackoverflow.
        // these rects are probably colliding
      }

			isLeaf = false;
	
			// TODO: could make this cleaner by precalculating width/height stuff.
			nw = new QuadTree(x, y, width / 2, height / 2);
			ne = new QuadTree(x + (width / 2), y, width / 2, height / 2);
			sw = new QuadTree(x, y + (height / 2), width / 2, height / 2);
			se = new QuadTree(x + (width / 2), y + (height / 2), width / 2, height / 2);
      nw->depth = this->depth+1;
      ne->depth = this->depth+1;
      sw->depth = this->depth+1;
      se->depth = this->depth+1;

			// For now, I'm going to add the point to every tree
			// which it technically belongs to. I believe this
			// is how this is supposed to work.
			for (int i = 0; i < points.size(); i++) {
				if (nw->insideOf(points[i])) {
					nw->insert(points[i]); // A stackoverflow points to here.
					// I think its because insideOf function is shitty right now.
					// Because this insert infinitely loops on the second
					// square in the points list.
				}
				if (ne->insideOf(points[i])) {
					ne->insert(points[i]);
				}
				if (sw->insideOf(points[i])) {
					sw->insert(points[i]);
				}
				if (se->insideOf(points[i])) {
					se->insert(points[i]);
				}
			}
			// I'm not sure if I'm supposed to do this, but
			// am going with it for now because it makes
			// more intuitive sense to me. 
			// (aka leafs can only have points).
			points.clear();
		}
	}
}

bool QuadTree::insideOf(Dot* point) {
	SDL_FRect* rect = point->rect;

	/*if ((pos->x < (x + width) && pos->x > x)
		&& (pos->y < (y + height) && pos->y > y)) {
		return true;
	}
	return false;*/

	//bool collisionX = rect->x <= x + width && rect->x + rect->w <= x;
	//bool collisionY = rect->y <= y + height && rect->y + rect->h <= y;

	bool colX = ((rect->x + rect->w) >= x) && ((x + width) >= rect->x);
	bool colY = ((rect->y + rect->h) >= y) && ((y + height) >= rect->y);

	return colX && colY;

}

void QuadTree::draw(SDL_Renderer* renderer) {
	SDL_Rect rect = { x,y,width,height };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	
	// Because there's only points in leafs (as of rn),
	// only check if this is a leaf.
	if (isLeaf) {
		for (int i = 0; i < points.size(); i++) {
			//SDL_RenderDrawPoint(renderer, points[i].x, points[i].y);
			//SDL_FRect frect = { points[i]->rect->x, points[i]->pos->y, 4, 4 };
			//SDL_RenderDrawRectF(renderer, &frect);
			//SDL_RenderFillRectF(renderer, points[i]->rect);
      SDL_RenderDrawRectF(renderer, points[i]->rect);
		}
	}
	else {
		nw->draw(renderer);
		ne->draw(renderer);
		sw->draw(renderer);
		se->draw(renderer);
	}
}

void QuadTree::update() {
	/*
	* NOTE: at some point I'm going to have to check if
	* all of a QuadTree's leafs are empty. If so, then we 
	* change that QuadTree back into a leaf. 
	*/

	if (isLeaf) {
		for (int i = 0; i < points.size(); i++) {
			if (!insideOf(points[i])) {

			}
		}
	}
	else {
		nw->update();
		ne->update();
		sw->update();
		se->update();
	}
}

std::vector<QuadTree*> QuadTree::getLeafs(Dot* dot){
	// If not leaf; call getLeaf on children which contain rect.
  // Else, return yourself in a vector.
  std::vector<QuadTree*> trees;
	if (!isLeaf) {
		if (nw->insideOf(dot)) {
      std::vector<QuadTree*> newTrees = nw->getLeafs(dot);
      trees.insert(trees.end(), newTrees.begin(), newTrees.end());
    }
		if (ne->insideOf(dot)) {
      std::vector<QuadTree*> newTrees = ne->getLeafs(dot);
      trees.insert(trees.end(), newTrees.begin(), newTrees.end());
		}
		if (sw->insideOf(dot)) {
			std::vector<QuadTree*> newTrees = sw->getLeafs(dot);
      trees.insert(trees.end(), newTrees.begin(), newTrees.end());
		}
		if (se->insideOf(dot)) {
			std::vector<QuadTree*> newTrees = se->getLeafs(dot);		
      trees.insert(trees.end(), newTrees.begin(), newTrees.end());
    }
	} else {
    trees.push_back(this);
  }
    return trees;
}

/*
* What's next?
* 
* I wrote an insert function (unsure of correctness) and a 
* draw function (unsure of correctness).
* 
* draw() shows two initial points added in their own squares
* seemingly correctly. Once two more are added (which are
* intentionally around one of the first two points), then
* it stops drawing the points for some reason.
* 
* Briefly debugged and didn't really see wtf was going on.
* 
* Debug this and have it draw correctly/insert correctly.
* (Sorry its not as fun as yesterdays, but it'll be
* satisfying to see it working with a lot of points.)
*/
