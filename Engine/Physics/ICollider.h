#pragma once
#include "CollisionInfo.h"

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
};