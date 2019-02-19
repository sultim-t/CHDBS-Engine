#pragma once

class ICollider
{
public:
	virtual bool Intersect(const ICollider &col) const = 0;
	virtual IShape &GetShape() const = 0;
};