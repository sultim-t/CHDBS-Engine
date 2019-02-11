#include "Input.h"

float Input::MouseX;
float Input::MouseY;
float Input::Scroll;
float Input::MouseXOffset;
float Input::MouseYOffset;
float Input::MouseSensitivity = 50.0f;

bool Input::IsPressed(int keycode)
{
	return ContextWindow::Instance().IsPressed(keycode);
}

bool Input::IsReleased(int keycode)
{
	return ContextWindow::Instance().IsReleased(keycode);
}
