#pragma once
#include "ICollider.h"
#include <Engine/Math/AABB.h>

class AABBCollider : public ICollider
{
private:
	AABB aabb;

public:
	// Empty constructor
	AABBCollider();
	AABBCollider(const AABB &aabb);
	
	//void SetTransform(const Transform *t);

	// Get AABB with current transformations
	AABB GetAABB() const;
	// Get current AABB
	AABB &GetAABBRef();

public:
	ColliderType	GetColliderType() const override;
	bool			Intersect(const ICollider & col, CollisionInfo &info) const override;
	Sphere			GetBoundingSphere() const override;
};