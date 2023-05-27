#include "Dot.h"

Dot::Dot(float x, float y) {
	pos = new SDL_FPoint{ x, y };

	xVel = (float)(rand()) / (float)(rand());
	yVel = (float)(rand()) / (float)(rand());

	xVel *= 10;
	yVel *= 10;

}

Dot::~Dot() {

}

void Dot::simulate(float dt) {
	pos->x += xVel * dt;
	pos->y += yVel * dt;
}