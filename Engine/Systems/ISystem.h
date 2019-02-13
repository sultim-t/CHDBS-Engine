#pragma once

class ISystem
{
	friend class Engine;

private:
	virtual void Init() = 0;
	virtual void Update() = 0;
};