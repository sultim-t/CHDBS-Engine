#pragma once
#include "Vector.h"
#include "IShape.h"

class AABB : public IShape
{
private:
	Vector3 minBounds;
	Vector3 maxBounds;

public:
	// Empty construcor
	AABB();
	AABB(const Vector3 &vec);
	AABB(const Vector3 &center, const float radius);
	AABB(const Vector3 &vec1, const Vector3 &vec2);

	bool IsEmpty() const;
	void Nullify();
	void Move(const Vector3 &vec);

	// Is this AABB contains other one?
	bool Contains(const AABB &aabb) const;
	// Is this AABB contained in other one?
	bool IsContained(const AABB &aabb) const;

	// Is this AABB intersected with the other one?
	bool Intersect(const AABB &aabb) const;
	// Is this AABB intersected with the sphere?
	bool Intersect(const Sphere &sphere) const;

	static AABB GetUnion(const AABB &aabb1, const AABB &aabb2);
	static AABB GetIntersection(const AABB &aabb1, const AABB &aabb2);

	AABB GetUnion(const AABB &aabb) const;
	AABB GetIntersection(const AABB &aabb) const;

	inline const Vector3 GetCenter() const;
	inline const Vector3 GetSize() const;
	inline const Vector3 &GetMin() const;
	inline const Vector3 &GetMax() const;

	inline void SetMin(const Vector3 &min);
	inline void SetMax(const Vector3 &max);

	inline bool Intersect(const IShape &shape) const override;
	inline ShapeType GetShapeType() const override;
};

inline const Vector3 AABB::GetCenter() const
{
	return (minBounds + maxBounds) * 0.5f;
}

inline const Vector3 AABB::GetSize() const
{
	return maxBounds - minBounds;
}

inline const Vector3 &AABB::GetMin() const
{
	return minBounds;
}

inline const Vector3 &AABB::GetMax() const
{
	return maxBounds;
}

inline void AABB::SetMin(const Vector3 &min)
{
	minBounds = min;
}

inline void AABB::SetMax(const Vector3 &max)
{
	maxBounds = max;
}

inline bool AABB::Intersect(const IShape &shape) const
{
	switch (shape.GetShapeType())
	{
	case ShapeType::AABB:
		return Intersect((AABB&)shape);
	case ShapeType::Sphere:
		return Intersect((Sphere&)shape);
	default:
		ASSERT(0);
		return false;
	}
}

inline ShapeType AABB::GetShapeType() const
{
	return ShapeType::AABB;
}