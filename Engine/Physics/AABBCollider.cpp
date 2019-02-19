#include "AABBCollider.h"

AABBCollider::AABBCollider(const AABB &aabb, const Vector3 &offset)
{
	this->aabb = aabb;
	this->offset = offset;
}

bool AABBCollider::Intersect(const ICollider &col) const
{
	aabb.Intersect(col.GetShape());
}