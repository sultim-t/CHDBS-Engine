#pragma once
#include "CollisionInfo.h"
#include <Engine/Math/Sphere.h>

enum class ColliderType
{
	AABB,
	Sphere,
	Mesh
};

class ICollider
{
public:
	virtual bool Intersect(const ICollider &col, CollisionInfo &info) const = 0;
	virtual ColliderType GetColliderType() const = 0;

	// Every collider have a bounding sphere to precheck collisions
	virtual Sphere GetBoundingSphere() const = 0;
};