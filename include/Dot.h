#pragma once
#include <SDL.h>
#include <cstdlib>

class Dot {
public:
	// This class is basically a wrapper around
	// this SDL_FPoint.
	SDL_FRect* rect;

	const static int SPEED = 10;
  static int ID_COUNTER; 

	float xVel, yVel;

	// min is just implied to be 0.
	// @bad: this is hardcoded, later can change to
	// size of area during runtime.
	const static int X_MAX = 1280;
	const static int Y_MAX = 720;
 
  int id;

	Dot(float x, float y, float w, float h);
	~Dot();

	void simulate(float dt);

};

