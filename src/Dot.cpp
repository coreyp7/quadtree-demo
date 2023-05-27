#include "Dot.h"

Dot::Dot(float x, float y) {
	pos.x = x;
	pos.y = y;

	xVel = (float)(rand()) / (float)(rand());
	yVel = (float)(rand()) / (float)(rand());



}

Dot::~Dot() {

}

void Dot::simulate(float dt) {
	pos.x += xVel * dt;
	pos.y += yVel * dt;
}