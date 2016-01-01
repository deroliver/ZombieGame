#include "Timing.h"

#include <SDL/SDL.h>

namespace gengine {

FpsLimiter::FpsLimiter() {

}


void FpsLimiter::init(float maxFPS) {
	setMaxFPS(maxFPS);
}


void FpsLimiter::setMaxFPS(float maxFPS) {
	_maxFPS = maxFPS;
}


void FpsLimiter::beginFrame() {
	_startTicks = SDL_GetTicks();
}


// Returns the FPS
float FpsLimiter::end() {
	calculateFPS();

	float frameTicks = SDL_GetTicks() - _startTicks;
	// Limit the FPS to max FPS
	if ((1000.0f / _maxFPS) > frameTicks) {
		SDL_Delay((1000.0f / _maxFPS) - frameTicks);
	}

	return _fps;
}

void FpsLimiter::calculateFPS() {
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();
	float currTicks;

	currTicks = SDL_GetTicks();

	_frameTime = currTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currTicks;

	int count;

	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = NUM_SAMPLES;
	}
	else {
		count = NUM_SAMPLES;
	}


	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}


	frameTimeAverage /= count;

	if (frameTimeAverage > 0) {
		_fps = 1000.0f / frameTimeAverage;
	}
	else {
		_fps = 60.0f;
	}
}


}