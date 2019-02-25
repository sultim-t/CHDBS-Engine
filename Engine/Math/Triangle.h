#pragma once
#include "Vector.h"

class Triangle
{
public:
	const Vector3 &A, &B, &C;

public:
	Triangle(const Vector3 & a, const Vector3 & b, const Vector3 & c);

public:
	// Get closest point on triangle abc
	static Vector3 GetClosestPoint(const Vector3 &p, const Vector3 &a, const Vector3 &b, const Vector3 &c);
	// Get closest point on this triangle
	inline Vector3 GetClosestPoint(const Vector3 &p) const;
	// Get cartesian coords from barycentric
	inline Vector3 GetCartesian(const Vector3 &barycentric) const;
};

inline Vector3 Triangle::GetClosestPoint(const Vector3 & p) const
{
	return GetClosestPoint(p, A, B, C);
}

inline Vector3 Triangle::GetCartesian(const Vector3 & barycentric) const
{
	return A * barycentric[0] + B * barycentric[1] + C * barycentric[2];
}
