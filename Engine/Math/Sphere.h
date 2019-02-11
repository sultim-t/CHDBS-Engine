#pragma once

#include "Vector.h"
//#include "..\Base\Typedef.h"

class Sphere
{
private:
	Vector3 center;
	float radius;
public:
	Sphere();
	Sphere(const float radius);
	Sphere(const Vector3 &center, const float radius);

	bool Contains(Sphere &sphere) const;
	bool Contacts(const Sphere &sphere) const;
	// bool ContactsAABB(const AABB center, const float radius);

	static Sphere &Union(const Sphere &sphere1, Sphere &sphere2);
	static Sphere &Intersect(Sphere &sphere1, Sphere &sphere2);

	Sphere &GetUnion(Sphere &sphere);
	Sphere &GetIntersection(Sphere &sphere);

	const Vector3 GetCenter() const;
	const float GetRadius() const;

	void SetCenter(Vector3 &vec);
	void SetRadius(const float radius);
};