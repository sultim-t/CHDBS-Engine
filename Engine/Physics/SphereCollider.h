#pragma once
#include "ICollider.h"
#include <Engine/Math/Transform.h>
#include <Engine/Math/Sphere.h>

class SphereCollider : public ICollider
{
private:
	// Main shape
	Sphere sphere;

	// Every dynamic collider must have tranformation
	const Transform *t;

public:
	// Empty constructor
	inline SphereCollider();
	inline SphereCollider(const Sphere &sphere);

	inline void SetTransform(const Transform *t);

	// Get sphere with current transformations
	inline Sphere GetSphere();
	// Get sphere with current transformations
	inline const Sphere GetSphere() const;
	// Get current sphere
	inline Sphere &GetSphereRef();

	inline ColliderType GetColliderType() const override;
	bool Intersect(const ICollider &col, CollisionInfo &info) const override;
	inline Sphere GetBoundingSphere() const override;
};

inline SphereCollider::SphereCollider()
{ }

inline SphereCollider::SphereCollider(const Sphere &source) : sphere(source) { }

inline void SphereCollider::SetTransform(const Transform * t)
{
	this->t = t;
}

inline Sphere SphereCollider::GetSphere()
{
	return Sphere(sphere.GetCenter() + t->GetPosition(), sphere.GetRadius());
}

inline const Sphere SphereCollider::GetSphere() const
{
	return Sphere(sphere.GetCenter() + t->GetPosition(), sphere.GetRadius());
}

inline Sphere & SphereCollider::GetSphereRef()
{
	return sphere;
}

inline ColliderType SphereCollider::GetColliderType() const
{
	return ColliderType::Sphere;
}

inline Sphere SphereCollider::GetBoundingSphere() const
{
	return Sphere(sphere.GetCenter() + t->GetPosition(), sphere.GetRadius());
}