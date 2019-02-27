#pragma once
#include "ICollider.h"
#include "MeshCollider.h"
#include <Engine/Math/AABB.h>
#include <Engine/Math/Intersection.h>

class AABBCollider : public ICollider
{
private:
	AABB aabb;

public:
	// Empty constructor
	inline AABBCollider();
	inline AABBCollider(const AABB &aabb);
	
	inline AABB &GetAABB();
	inline const AABB &GetAABB() const;

	inline ColliderType GetColliderType() const override;
	bool Intersect(const ICollider & col) const override;
};

inline AABBCollider::AABBCollider()
{ }

inline AABBCollider::AABBCollider(const AABB &aabb)
{
	this->aabb = aabb;
}

inline AABB &AABBCollider::GetAABB()
{
	return aabb;
}

inline const AABB &AABBCollider::GetAABB() const
{
	return aabb;
}

inline ColliderType AABBCollider::GetColliderType() const
{
	return ColliderType::AABB;
}