#include "Input.h"
#include <Engine\Rendering\ContextWindow.h>

float Input::MouseX;
float Input::MouseY;
float Input::Scroll;
float Input::MouseXOffset;
float Input::MouseYOffset;
float Input::MouseSensitivity = 1.0f;

bool Input::IsPressed(Keycode keycode)
{
	return ContextWindow::Instance().IsPressed((int)keycode);
}

bool Input::IsReleased(Keycode keycode)
{
	return ContextWindow::Instance().IsReleased((int)keycode);
}

bool Input::IsPressedMouse(Keycode keycode)
{
	return ContextWindow::Instance().IsPressedMouse((int)keycode);
}