#include "Time.h"
#include <GLFW\glfw3.h>

float Time::deltaTime = 0;
float Time::lastFrame = 0;

void Time::Calculate()
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

float Time::GetTime()
{
	return (float)glfwGetTime();
}

float Time::GetDeltaTime()
{
	return deltaTime;
}
