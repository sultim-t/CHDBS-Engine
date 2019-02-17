#pragma once

#include "Vector.h"
//#include "..\Base\Typedef.h"

class Sphere
{
private:
	Vector3 center;
	float radius;

public:
	// Empty constructor
	Sphere();
	// Construct sphere with center in 0 and given radius
	Sphere(float radius);
	// Construct sphere with given center and radius
	Sphere(const Vector3 &center, float radius);

	bool Contains(const Sphere &sphere) const;
	bool Contacts(const Sphere &sphere) const;
	// bool ContactsAABB(const AABB center, const float radius);

	static Sphere GetUnion(const Sphere &sphere1, const  Sphere &sphere2);
	static Sphere GetIntersection(const Sphere &sphere1, const Sphere &sphere2);

	Sphere GetUnion(const Sphere &sphere) const;
	Sphere GetIntersection(const Sphere &sphere) const;

	// Union this sphere with another
	void Union(const Sphere &sphere);
	// Intersect this sphere with another
	void Intersect(const Sphere &sphere);

	const Vector3 &GetCenter() const;
	float GetRadius() const;

	void SetCenter(const Vector3 &vec);
	void SetRadius(float radius);
};