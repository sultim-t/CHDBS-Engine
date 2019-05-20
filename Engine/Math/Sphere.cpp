#include "Sphere.h"
#include "AABB.h"

Sphere::Sphere()
{}

Sphere::Sphere(const float radius) : Sphere::Sphere((0.0f, 0.0f, 0.0f), radius)
{}

Sphere::Sphere(const Vector3 &center, const float radius)
{
	ASSERT(radius > 0);

	this->center = center;
	this->radius = radius;
}

bool Sphere::Contains(const Sphere &sphere) const
{
	float sqrDist = center.LengthSqr(sphere.center);
	float delta = radius - sphere.radius;

	return sqrDist <= delta * delta;
}

Sphere Sphere::GetUnion(const Sphere &sphere1, const Sphere &sphere2)
{
	Vector3 newCenter = (sphere1.center + sphere2.center) * 0.5f;
	float newRadius = (sphere1.radius + sphere2.radius) * 0.5f;

	return Sphere(newCenter, newRadius);
}

Sphere Sphere::GetIntersection(const Sphere &sphere1, const Sphere &sphere2)
{
	using namespace Mathf;

	Vector3 newCenter = (sphere1.center + sphere2.center) * 0.5f;
	float newRadius = Abs(sphere2.radius - sphere1.radius) * 0.5f;

	return Sphere(newCenter, newRadius);
}

Sphere Sphere::GetUnion(const Sphere &sphere) const
{
	return GetUnion(*this, sphere);
}

Sphere Sphere::GetIntersection(const Sphere &sphere) const
{
	return GetIntersection(*this, sphere);
}

/*void Sphere::Union(const Sphere &sphere)
{
	*this = GetUnion(*this, sphere);
}

void Sphere::Intersect(const Sphere &sphere)
{
	*this = GetIntersection(*this, sphere);
}*/