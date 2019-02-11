#include "Sphere.h"

Sphere::Sphere() : Sphere::Sphere((0.0f,0.0f,0.0f), 1.0f)
{}

Sphere::Sphere(const float radius) : Sphere::Sphere((0.0f, 0.0f, 0.0f), radius)
{}

Sphere::Sphere(const Vector3 & center, const float radius)
{
	ASSERT(radius <= 0);

	this->center = center;
	this->radius = radius;
}

bool Sphere::Contains(Sphere & sphere) const
{
	float sqrDist = center.LengthSqr(sphere.center);
	float delta = radius - sphere.radius;

	return sqrDist <= delta * delta;
}

bool Sphere::Contacts(const Sphere & sphere) const
{
	float sqrDist = center.LengthSqr(sphere.center);
	float sum = radius + sphere.radius;

	return sqrDist < sum * sum;
}

Sphere & Sphere::Union(const Sphere & sphere1, Sphere & sphere2)
{
	Vector3 newCenter = (sphere1.center + sphere2.center) * 0.5f;
	float newRadius = (sphere1.radius + sphere2.radius) * 0.5f;

	return *(new Sphere(newCenter, newRadius));
}

Sphere & Sphere::Intersect(Sphere & sphere1, Sphere & sphere2)
{
	Vector3 newCenter = (sphere1.center + sphere2.center) * 0.5f;
	float newRadius = ABS(sphere2.radius - sphere1.radius) * 0.5f;

	return *(new Sphere(newCenter, newRadius));
}

Sphere & Sphere::GetUnion(Sphere & sphere)
{
	return Union(*this, sphere);
}

Sphere & Sphere::GetIntersection(Sphere & sphere)
{
	return Intersect(*this, sphere);
}

const Vector3 Sphere::GetCenter() const
{
	return center;
}

const float Sphere::GetRadius() const
{
	return radius;
}

void Sphere::SetCenter(Vector3 & vec)
{
	center = vec;
}

void Sphere::SetRadius(const float radius)
{
	ASSERT(radius <= 0);

	this->radius = radius;
}
