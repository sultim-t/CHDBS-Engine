#pragma once
#include "Vector.h"

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

	// Does this sphere contain the other one?
	bool Contains(const Sphere &sphere) const;

	static Sphere GetUnion(const Sphere &sphere1, const Sphere &sphere2);
	static Sphere GetIntersection(const Sphere &sphere1, const Sphere &sphere2);

	Sphere GetUnion(const Sphere &sphere) const;
	Sphere GetIntersection(const Sphere &sphere) const;

	inline const Vector3 &GetCenter() const;
	inline float GetRadius() const;

	inline void Move(const Vector3 &vec);
	inline void SetCenter(const Vector3 &vec);
	inline void SetRadius(float radius);
};

inline const Vector3 &Sphere::GetCenter() const
{
	return center;
}

inline float Sphere::GetRadius() const
{
	return radius;
}

inline void Sphere::Move(const Vector3 &vec)
{
	center += vec;
}

inline void Sphere::SetCenter(const Vector3 &vec)
{
	center = vec;
}

inline void Sphere::SetRadius(float radius)
{
	ASSERT(radius > 0);

	this->radius = radius;
}