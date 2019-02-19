#pragma once
#include "Vector.h"
#include "IShape.h"

class Sphere : public IShape
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
	
	// Does this sphere intersect the other one?
	bool Intersect(const Sphere &sphere) const;
	// Does this sphere intersect the AABB?
	bool Intersect(const AABB &aabb) const;

	static Sphere GetUnion(const Sphere &sphere1, const Sphere &sphere2);
	static Sphere GetIntersection(const Sphere &sphere1, const Sphere &sphere2);

	Sphere GetUnion(const Sphere &sphere) const;
	Sphere GetIntersection(const Sphere &sphere) const;

	inline const Vector3 &GetCenter() const;
	inline float GetRadius() const;

	inline void SetCenter(const Vector3 &vec);
	inline void SetRadius(float radius);

	inline bool Intersect(const IShape &shape) const override;
	inline ShapeType GetShapeType() const override;
};

inline const Vector3 &Sphere::GetCenter() const
{
	return center;
}

inline float Sphere::GetRadius() const
{
	return radius;
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

inline bool Sphere::Intersect(const IShape & shape) const
{
	switch (shape.GetShapeType())
	{
	case ShapeType::AABB:
		return Intersect((AABB&)shape);
	case ShapeType::Sphere:
		return Intersect((Sphere&)shape);
	default:
		ASSERT(0);
	}
}

inline ShapeType Sphere::GetShapeType() const
{
	return ShapeType::Sphere;
}
