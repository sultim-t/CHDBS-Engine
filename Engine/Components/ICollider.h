#pragma once

class ICollider
{
public:
	virtual bool Intersect(const ICollider &col) = 0;
};