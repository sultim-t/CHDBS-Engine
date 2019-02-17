#pragma once
#include "Vector.h"
#include "Sphere.h"

// Plane represented as: Ax + By + Cz + D = 0
class Plane
{
private:
	// A, B, C
	Vector3 normal;
	// D
	float distance;

public:
	// Empty constructor
	inline Plane();
	// Construct plane from normal and distance from 0
	inline Plane(const Vector3 &normal, float distance);
	// Construct plane from normal and distance from 0
	inline Plane(const Vector4 &nd);
	// Construct plane as Ax + By + Cz + D = 0
	inline Plane(float A, float B, float C, float D);
	// Construct plane from 3 points on it, counter clockwise order
	inline Plane(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);
	// Construct plane from normal and point on plane
	inline Plane(const Vector3 &normal, const Vector3 &point); 

	// Is point in front of the plane?
	inline bool IsInside(const Vector3 &point) const;
	// Is sphere in front of the plane?
	inline bool IsInside(const Sphere &sphere) const;
	// Is sphere in front of the plane?
	inline bool IsInside(const Vector3 &point, float radius) const;

	inline float PlaneDot(const Vector3 &point) const;
};

inline Plane::Plane()
{ }

inline Plane::Plane(const Vector3 & normal, float distance)
{
	this->normal = normal.GetNormalized();
	this->distance = distance;
}

inline Plane::Plane(const Vector4 & nd)
{
	this->normal[0] = nd[0];
	this->normal[1] = nd[1];
	this->normal[2] = nd[2];

	this->normal.Normalize();

	this->distance = nd[3];
}

inline Plane::Plane(float A, float B, float C, float D)
{
	this->normal[0] = A;
	this->normal[1] = B;
	this->normal[2] = C;

	this->normal.Normalize();

	this->distance = D;
}

inline Plane::Plane(const Vector3 & p1, const Vector3 & p2, const Vector3 & p3)
{
	Vector3 n = Vector3::Cross((p3 - p2), (p2 - p1));
	*this = Plane(n, p1);
}

inline Plane::Plane(const Vector3 & normal, const Vector3 & point)
{
	this->normal = normal.GetNormalized();
	this->distance = Vector3::Dot(this->normal, point);
}

inline bool Plane::IsInside(const Vector3 & point) const
{
	float distToPoint = PlaneDot(point);
	return distToPoint >= 0;
}

inline bool Plane::IsInside(const Sphere & sphere) const
{
	return IsInside(sphere.GetCenter(), sphere.GetRadius());
}

inline bool Plane::IsInside(const Vector3 & point, float radius) const
{
	float distToCenter = PlaneDot(point);
	return distToCenter >= -radius;
}

inline float Plane::PlaneDot(const Vector3 & point) const
{
	return normal[0] * point[0] + normal[1] * point[1] + normal[2] * point[2] + distance;
}
