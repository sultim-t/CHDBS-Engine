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

	PhysicMaterial physicMaterial;

public:
	// Empty constructor
	AABBCollider();
	AABBCollider(const AABB &aabb);
	
	void SetTransform(const Transform *t);

	// Get AABB with current transformations
	AABB GetAABB();
	// Get AABB with current transformations
	const AABB GetAABB() const;
	// Get current AABB
	AABB &GetAABBRef();

	ColliderType GetColliderType() const override;
	bool Intersect(const ICollider & col, CollisionInfo &info) const override;
	Sphere GetBoundingSphere() const override;
	PhysicMaterial GetPhysicMaterial() const override;
};