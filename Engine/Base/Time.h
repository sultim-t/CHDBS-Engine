#pragma once

class Time
{
	friend class Engine;

private:
	static float lastFrame;
	static float deltaTime;
	static float fixedDeltaTime;
	static float fdtAccum;

public:
	// Must be called only on frame change
	static void Calculate();
	static bool CalculateFixedDelta ();

public:
	// Current time
	static float GetTime();
	// Delta time (frame update)
	static float GetDeltaTime();
	// Fixed delta time (physics update)
	static float GetFixedDeltaTime();
};