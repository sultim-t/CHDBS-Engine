#include "InputSystem.h"
#include <Engine/Input/Input.h>

void InputSystem::Init()
{ }

void InputSystem::Update()
{
	Input::MouseXOffset = Input::MouseX - prevMouseX;
	// reversed, y must be from bottom to top
	Input::MouseYOffset = prevMouseY - Input::MouseY;

	prevMouseX = Input::MouseX;
	prevMouseY = Input::MouseY;
}

InputSystem & InputSystem::Instance()
{
	static InputSystem instance;
	return instance;
}
