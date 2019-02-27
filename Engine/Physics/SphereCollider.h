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

	inline ColliderType GetColliderType() const override;
	bool Intersect(const ICollider &col) const override;
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

inline ColliderType SphereCollider::GetColliderType() const
{
	return ColliderType::Sphere;
}