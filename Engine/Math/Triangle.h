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
};