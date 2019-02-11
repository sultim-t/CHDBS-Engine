#pragma once

#include "Vector.h"

class AABB
{
private:
	Vector3 minBounds;
	Vector3 maxBounds;
public:
	AABB();
	AABB(const Vector3 &vec);
	AABB(const Vector3 &center, const float radius);
	AABB(const Vector3 &vec1, const Vector3 &vec2);

	bool IsEmpty() const;
	void Nullify();
	void Move(Vector3 &vec);
	bool Contains(AABB &aabb) const;
	bool IsContained(AABB &aabb) const;
	bool Contacts(const AABB &aabb) const;
	// bool Contacts(const AABB &aabb, const float eps) const;
	bool ContactsSphere(const Vector3 center, const float radius) const;

	static AABB &Union(const AABB &aabb1, const AABB &aabb2);
	static AABB &Intersect(const AABB &aabb1, const AABB &aabb2);

	AABB &GetUnion(const AABB &aabb);
	AABB &GetIntersection(const AABB &aabb);

	const Vector3 &GetCenter() const;
	const Vector3 GetSize() const;
	const Vector3 &GetMin() const;
	const Vector3 &GetMax() const;
};