#pragma once

namespace gengine {

class FpsLimiter {
public:
	FpsLimiter();
	void init(float maxFPS);

	void setMaxFPS(float maxFPS);

	void beginFrame();
	
	// Returns the FPS
	float end();

private:
	void calculateFPS();

	float _fps;
	float _maxFPS;
	float _frameTime;
	unsigned int _startTicks;
};

}
