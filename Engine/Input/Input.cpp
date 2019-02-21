#include "Input.h"

float Input::MouseX;
float Input::MouseY;
float Input::Scroll;
float Input::MouseXOffset;
float Input::MouseYOffset;
float Input::MouseSensitivity = 1.0f;

bool Input::IsPressed(const Keycode keycode)
{
	return ContextWindow::Instance().IsPressed((int)keycode);
}

bool Input::IsReleased(const Keycode keycode)
{
	return ContextWindow::Instance().IsReleased((int)keycode);
}
