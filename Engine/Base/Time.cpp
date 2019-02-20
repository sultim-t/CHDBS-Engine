#include "Time.h"
#include <GLFW\glfw3.h>

float Time::deltaTime = 0;
float Time::lastFrame = 0;
float Time::fixedDeltaTime = 1.0f / 60.0f;
float Time::fdtAccum = 0;

void Time::Init()
{
	lastFrame = GetTime();
}

void Time::Calculate()
{
	float currentFrame = GetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	fdtAccum += deltaTime;
}

bool Time::ToFixedUpdate()
{
	if (fdtAccum >= fixedDeltaTime)
	{
		fdtAccum -= fixedDeltaTime;
		return true;
	}
	else
	{
		return false;
	}
}

float Time::GetTime()
{
	return (float)glfwGetTime();
}

float Time::GetDeltaTime()
{
	return deltaTime;
}

float Time::GetFixedDeltaTime()
{
	return fixedDeltaTime;
}
