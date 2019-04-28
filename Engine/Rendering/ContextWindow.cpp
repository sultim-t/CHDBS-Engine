#include "ContextWindow.h"
#include "OpenGL.h"
#include <Engine\Input\Input.h>
#include <GLFW\glfw3.h>

static GLFWwindow *window;

void FramebufferSizeÑallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

ContextWindow::ContextWindow()
{
}

ContextWindow::~ContextWindow()
{
}

void FramebufferSizeÑallback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	Input::MouseX = (float)xpos;
	Input::MouseY = (float)ypos;
}

void ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	Input::Scroll = (float)yoffset;
}

ContextWindow &ContextWindow::Instance()
{
	static ContextWindow instance;
	return instance;
}

void ContextWindow::Init(const char* name, int width, int height)
{	
	// GLFW INIT
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (window == NULL)
	{
		Terminate();
	}

	Input::MouseX = width / 2.0f;
	Input::MouseY = height / 2.0f;

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeÑallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW INIT
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Terminate();
	}

	this->width = width;
	this->height = height;

	glEnable(GL_DEPTH_TEST);
}

void ContextWindow::ResetViewport()
{
	glViewport(0, 0, this->width, this->height);
}

void ContextWindow::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void ContextWindow::PollEvents()
{
	glfwPollEvents();
}

void ContextWindow::Terminate()
{
	glfwTerminate();
}

bool ContextWindow::ShouldClose()
{
	return glfwWindowShouldClose(window) != 0;
}

void ContextWindow::RequestClose()
{
	glfwSetWindowShouldClose(window, true);
}

int ContextWindow::GetWidth()
{
	return width;
}

int ContextWindow::GetHeight()
{
	return height;
}

bool ContextWindow::IsReleased(int keyCode)
{
	return glfwGetKey(window, keyCode) == GLFW_RELEASE;
}

bool ContextWindow::IsPressed(int keyCode)
{
	return glfwGetKey(window, keyCode) == GLFW_PRESS;
}

bool ContextWindow::IsPressedMouse(int keyCode)
{
	return glfwGetMouseButton(window, keyCode) == GLFW_PRESS;
}
