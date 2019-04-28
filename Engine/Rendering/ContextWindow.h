#pragma once

//#include <GLFW\glfw3.h>

class ContextWindow
{
private:
	int width, height;

private:
	ContextWindow();
	~ContextWindow();

	ContextWindow(ContextWindow&) = delete;
	ContextWindow(ContextWindow&&) = delete;
	ContextWindow &operator=(const ContextWindow&) = delete;

public:
	static ContextWindow &Instance();

	void Init(const char* name, int width, int height);
	void ResetViewport();
	void SwapBuffers();
	void PollEvents();

	void Terminate();
	bool ShouldClose();
	void RequestClose();
	
	int GetWidth();
	int GetHeight();

	bool IsPressed(int keyCode);
	bool IsPressedMouse(int keyCode);
	bool IsReleased(int keyCode);
};