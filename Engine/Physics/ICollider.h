#pragma once

enum class ColliderType
{
	AABB,
	Sphere,
	Mesh
};

class ICollider
{
public:
	virtual bool Intersect(const ICollider &col) const = 0;
	virtual ColliderType GetColliderType() const = 0;
};