#pragma once

enum class ShapeType
{
	AABB,
	Sphere
};

class IShape
{
public:
	virtual bool Intersect(const IShape &shape) const = 0;
	virtual ShapeType GetShapeType() const = 0;
};