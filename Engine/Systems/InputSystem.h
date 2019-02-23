#pragma once

#include "ISystem.h"

class InputSystem : ISystem
{
private:
	float prevMouseX, prevMouseY;

public:
	void Init() override;
	void Update() override;

	static InputSystem &Instance();
};