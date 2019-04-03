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

	// Init plane from 3 points on it, counter clockwise order
	inline void FromPoints(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);

	// Returns closest point on the plane
	inline Vector3 GetClosestPoint(const Vector3 &from) const;

	// Is point in front of the plane?
	inline bool IsInFront(const Vector3 &point) const;
	// Does sphere touch front halfspace of the plane?
	inline bool IsInFront(const Sphere &sphere) const;
	// Does sphere touch front halfspace of the plane?
	inline bool IsInFront(const Vector3 &point, float radius) const;

	// Returns signed distance to point
	inline float PlaneDot(const Vector3 &point) const;

	inline const Vector3 &GetNormal() const;
	inline float GetD() const;

	inline void SetNormal(const Vector3 &n);
	inline void SetD(float d);
};

inline Plane::Plane()
{ }

inline Plane::Plane(const Vector3 &normal, float distance)
{
	this->normal = normal.GetNormalized();
	this->distance = distance;
}

inline Plane::Plane(const Vector4 &nd)
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

inline Plane::Plane(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
{
	FromPoints(p1, p2, p3);
}

inline Plane::Plane(const Vector3 &normal, const Vector3 &point)
{
	this->normal = normal.GetNormalized();
	this->distance = -Vector3::Dot(this->normal, point);
}

inline void Plane::FromPoints(const Vector3 & p1, const Vector3 & p2, const Vector3 & p3)
{
	normal = Vector3::Cross((p3 - p2), (p2 - p1));
	normal.Normalize();

	distance = -Vector3::Dot(normal, p1);
}

inline Vector3 Plane::GetClosestPoint(const Vector3 & from) const
{
	// ASSERT(normal.Length() == 1.0f);

	float t = PlaneDot(from);
	return from - normal * t;
}

inline bool Plane::IsInFront(const Vector3 &point) const
{
	float distToPoint = PlaneDot(point);
	return distToPoint >= 0;
}

inline bool Plane::IsInFront(const Sphere &sphere) const
{
	return IsInFront(sphere.GetCenter(), sphere.GetRadius());
}

inline bool Plane::IsInFront(const Vector3 &point, float radius) const
{
	float distToCenter = PlaneDot(point);
	return distToCenter >= -radius;
}

inline float Plane::PlaneDot(const Vector3 &point) const
{
	return Vector3::Dot(normal, point) + distance;
}

inline const Vector3 & Plane::GetNormal() const
{
	return normal;
}

inline float Plane::GetD() const
{
	return distance;
}

inline void Plane::SetNormal(const Vector3 & n)
{
	normal = n.GetNormalized();
}

inline void Plane::SetD(float d)
{
	distance = d;
}
