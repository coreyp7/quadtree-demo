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
	float oldX = pos->x;
	float oldY = pos->y;

	pos->x += xVel * dt;
	pos->y += yVel * dt;

	if (pos->x < 0 || pos->x > X_MAX) {
		xVel = -xVel;
		pos->x = oldX + (xVel * dt);
	}

	if (pos->y < 0 || pos->y > Y_MAX) {
		yVel = -yVel;
		pos->y = oldY + (yVel * dt);
	}
}