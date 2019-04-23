#pragma once
#include "Vector.h"

class AABB
{
private:
	Vector3 minBounds;
	Vector3 maxBounds;

public:
	// Empty construcor
	AABB();
	// Create bounding box Min=-vec, Max=vec
	AABB(const Vector3 &vec);
	// Create bounding box of sphere
	AABB(const Vector3 &center, const float radius);
	// Create bounding box of two arbitary vectors
	AABB(const Vector3 &vec1, const Vector3 &vec2);

	bool IsEmpty() const;
	void Nullify();
	void Move(const Vector3 &vec);

	// Is this AABB contains other one?
	bool Contains(const AABB &aabb) const;
	// Is this AABB contains a point?
	bool Contains(const Vector3 &point) const;
	// Is this AABB contained in other one?
	bool IsContained(const AABB &aabb) const;

	// Get bounding box of these AABBs
	static AABB GetUnion(const AABB &aabb1, const AABB &aabb2);
	static AABB GetIntersection(const AABB &aabb1, const AABB &aabb2);

	AABB GetUnion(const AABB &aabb) const;
	AABB GetIntersection(const AABB &aabb) const;

	// Returns closest point on the AABB
	Vector3 GetClosestPoint(const Vector3 &p) const;
	// Returns squared distance from point to AABB
	float DistanceSqrTo(const Vector3 &p) const;
	// Returns distance from point to AABB
	float DistanceTo(const Vector3 &p) const;

	inline Vector3 GetCenter() const;
	inline Vector3 GetSize() const;

	inline const Vector3 &GetMin() const;
	inline const Vector3 &GetMax() const;
	inline Vector3 &GetMin();
	inline Vector3 &GetMax();

	inline Vector3 GetExtent() const;

	inline void SetMin(const Vector3 &min);
	inline void SetMax(const Vector3 &max);
};

inline Vector3 AABB::GetCenter() const
{
	return (minBounds + maxBounds) * 0.5f;
}

inline Vector3 AABB::GetSize() const
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

inline Vector3 & AABB::GetMin()
{
	return minBounds;
}

inline Vector3 & AABB::GetMax()
{
	return maxBounds;
}

inline Vector3 AABB::GetExtent() const
{
	return (maxBounds - minBounds) * 0.5f;
}

inline void AABB::SetMin(const Vector3 &min)
{
	minBounds = min;
}

inline void AABB::SetMax(const Vector3 &max)
{
	maxBounds = max;
}