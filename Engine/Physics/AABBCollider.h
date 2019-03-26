#pragma once
#include "ICollider.h"
#include "MeshCollider.h"
#include <Engine/Math/AABB.h>
#include <Engine/Math/Intersection.h>

class AABBCollider : public ICollider
{
private:
	AABB aabb;
	const Transform *t;

public:
	// Empty constructor
	inline AABBCollider();
	inline AABBCollider(const AABB &aabb);
	
	inline void SetTransform(const Transform *t);

	// Get AABB with current transformations
	inline AABB GetAABB();
	// Get AABB with current transformations
	inline const AABB GetAABB() const;
	// Get current AABB
	inline AABB &GetAABBRef();

	inline ColliderType GetColliderType() const override;
	bool Intersect(const ICollider & col, CollisionInfo &info) const override;
	Sphere GetBoundingSphere() const override;
};

inline AABBCollider::AABBCollider()
{ }

inline AABBCollider::AABBCollider(const AABB &aabb)
{
	this->aabb = aabb;
}

inline void AABBCollider::SetTransform(const Transform *t)
{
	this->t = t;
}

inline AABB AABBCollider::GetAABB()
{
	AABB result = AABB(aabb);
	result.Move(t->GetPosition());

	return result;
}

inline const AABB AABBCollider::GetAABB() const
{
	AABB result = AABB(aabb);
	result.Move(t->GetPosition());

	return result;
}

inline AABB &AABBCollider::GetAABBRef()
{
	return aabb;
}

inline ColliderType AABBCollider::GetColliderType() const
{
	return ColliderType::AABB;
}

inline Sphere AABBCollider::GetBoundingSphere() const
{
	Vector3 center;
	float radius;

	center = aabb.GetCenter() + t->GetPosition();
	radius = aabb.GetSize().Length() * 0.5f;

	return Sphere(center, radius);
}