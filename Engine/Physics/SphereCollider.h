#pragma once
#include "ICollider.h"
#include <Engine/Math/Sphere.h>

class SphereCollider : public ICollider
{
private:
	Sphere sphere;

public:
	// Empty constructor
	inline SphereCollider();
	inline SphereCollider(const Sphere &sphere);

	inline Sphere &GetSphere();
	inline const Sphere &GetSphere() const;

	inline bool Intersect(const ICollider &col) const override;
	inline IShape &GetShape() const override;
};

inline SphereCollider::SphereCollider()
{ }

inline SphereCollider::SphereCollider(const Sphere &sphere)
{
	this->sphere = sphere;
}

inline Sphere &SphereCollider::GetSphere()
{
	return sphere;
}

inline const Sphere &SphereCollider::GetSphere() const
{
	return sphere;
}

inline bool SphereCollider::Intersect(const ICollider &col) const
{
	return sphere.Intersect(col.GetShape());
}

IShape &SphereCollider::GetShape() const
{
	return (IShape&)sphere;
}