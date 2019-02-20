#pragma once
#include "ICollider.h"
#include <Engine/Math/AABB.h>

class AABBCollider : public ICollider
{
private:
	AABB aabb;
	Vector3 offset;

public:
	// Empty constructor
	inline AABBCollider();
	inline AABBCollider(const AABB &aabb, const Vector3 &offset = Vector3(0.0f));
	
	inline AABB &GetAABB();
	inline const AABB &GetAABB() const;

	inline Vector3 &GetOffset();
	inline const Vector3 &GetOffset() const;

	inline bool Intersect(const ICollider &col) const override;
	inline IShape &GetShape() const override;
};

inline AABBCollider::AABBCollider()
{ }

inline AABBCollider::AABBCollider(const AABB &aabb, const Vector3 &offset)
{
	this->aabb = aabb;
	this->offset = offset;
}

inline AABB &AABBCollider::GetAABB()
{
	return aabb;
}

inline const AABB &AABBCollider::GetAABB() const
{
	return aabb;
}

inline Vector3 &AABBCollider::GetOffset()
{
	return offset;
}

inline const Vector3 &AABBCollider::GetOffset() const
{
	return offset;
}

inline bool AABBCollider::Intersect(const ICollider &col) const
{
	return aabb.Intersect(col.GetShape());
}

inline IShape &AABBCollider::GetShape() const
{
	return (IShape&)aabb;
}