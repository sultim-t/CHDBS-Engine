#pragma once

// Component will be updated each frame
class IUpdatable
{
public:
	virtual void Update() = 0;
};