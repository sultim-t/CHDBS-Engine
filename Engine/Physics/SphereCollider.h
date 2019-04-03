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

	PhysicMaterial physicMaterial;

public:
	// Empty constructor
	SphereCollider();
	SphereCollider(const Sphere &sphere);

	void SetTransform(const Transform *t);

	// Get sphere with current transformations
	Sphere GetSphere();
	// Get sphere with current transformations
	const Sphere GetSphere() const;
	// Get current sphere
	Sphere &GetSphereRef();

	ColliderType GetColliderType() const override;
	bool Intersect(const ICollider &col, CollisionInfo &info) const override;
	Sphere GetBoundingSphere() const override;
	PhysicMaterial GetPhysicMaterial() const override;
};