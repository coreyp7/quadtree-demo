#pragma once
#include <SDL.h>
#include <cstdlib>

class Dot {
public:
	// This class is basically a wrapper around
	// this SDL_FPoint.
	SDL_FPoint* pos;

	const static int SPEED = 10;

	float xVel, yVel;

	Dot(float x, float y);
	~Dot();

	void simulate(float dt);

};

