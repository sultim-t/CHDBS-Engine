#pragma once
#include "ICollider.h"
#include <Engine/Math/AABB.h>

class AABBCollider : public ICollider
{
private:
	AABB aabb;
	Vector3 offset;

public:
	AABBCollider(const AABB &aabb, const Vector3 &offset = Vector3(0.0f));
	
	inline AABB &GetAABB();
	inline const AABB &GetAABB() const;

	bool Intersect(const ICollider &col) const override;
};

inline AABB &AABBCollider::GetAABB()
{
	return aabb;
}

inline const AABB &AABBCollider::GetAABB() const
{
	return aabb;
}