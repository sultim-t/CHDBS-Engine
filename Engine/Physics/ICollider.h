#pragma once
#include "CollisionInfo.h"
#include "PhysicMaterial.h"
#include <Engine/Math/Sphere.h>

enum class ColliderType
{
	AABB,
	Sphere,
	Mesh
};

// Represents collider
class ICollider
{
public:
	// Get current collider type
	virtual ColliderType GetColliderType() const = 0;

	// Every collider have a bounding sphere to precheck collisions
	virtual Sphere GetBoundingSphere() const = 0;

	// Intersect this collider with other one, collision info is generated
	virtual bool Intersect(const ICollider &col, CollisionInfo &info) const = 0;

	// Get physic material of this collider
	virtual PhysicMaterial GetPhysicMaterial() const = 0;
};