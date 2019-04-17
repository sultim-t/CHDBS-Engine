#pragma once

// Component will be updated each fixed (physics) step
class IFixedUpdatable
{
public:
	virtual void FixedUpdate() = 0;
};