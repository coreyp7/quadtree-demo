#include "QuadTree.h"

// Set defaults to static limit stuff (these are good values I've found).
int QuadTree::LIMIT = 4;
int QuadTree::DEPTH_LIMIT = 5;

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

void QuadTree::insert(Entity* point) {

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
        // ignore; stop making new quadtrees.
        return; 
      }

			isLeaf = false;
	
			nw = new QuadTree(x, y, width / 2, height / 2);
			ne = new QuadTree(x + (width / 2), y, width / 2, height / 2);
			sw = new QuadTree(x, y + (height / 2), width / 2, height / 2);
			se = new QuadTree(x + (width / 2), y + (height / 2), width / 2, height / 2);
      // todo: put this in quadtree constructor.
      nw->depth = this->depth+1;
      ne->depth = this->depth+1;
      sw->depth = this->depth+1;
      se->depth = this->depth+1;

			// Add entity to each leaf it belongs to.
			for (int i = 0; i < points.size(); i++) {
				if (nw->insideOf(points[i])) {
					nw->insert(points[i]);
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

      // Only leafs can contain entities, so clear this tree's points.
			points.clear();
		}
	}
}

// Check if an entity is inside of this QuadTree.
bool QuadTree::insideOf(Entity* point) {
	SDL_FRect* rect = point->rect;

	bool colX = ((rect->x + rect->w) >= x) && ((x + width) >= rect->x);
	bool colY = ((rect->y + rect->h) >= y) && ((y + height) >= rect->y);

	return colX && colY;
}

// Render this QuadTree and all of its children.
// Also draws all contained entities.
void QuadTree::draw(SDL_Renderer* renderer) {
	SDL_Rect rect = { x,y,width,height };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	
	if (isLeaf) {
		for (int i = 0; i < points.size(); i++) {
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


// Will return all leafs which the passed in Entity
// is contained in. (An object can be in more than one leaf at a time).
std::vector<QuadTree*> QuadTree::getLeafs(Entity* dot){

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

