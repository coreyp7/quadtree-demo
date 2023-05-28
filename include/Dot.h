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

	// min is just implied to be 0.
	const static int X_MAX = 1080;
	const static int Y_MAX = 720;

	Dot(float x, float y);
	~Dot();

	void simulate(float dt);

};

