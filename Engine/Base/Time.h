#pragma once

class Time
{
	friend class Engine;

private:
	static float lastFrame;
	static float deltaTime;

public:
	// Must be called only on frame change
	static void Calculate();

public:
	// Current time
	static float GetTime();
	// Delta time
	static float GetDeltaTime();
};