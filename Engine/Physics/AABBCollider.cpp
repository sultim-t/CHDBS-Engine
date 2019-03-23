#include "AABBCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"

bool AABBCollider::Intersect(const ICollider & col, CollisionInfo &info) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();

		if (!Intersection::AABBAABB(aabb, other, info.Point, info.Normal, info.Penetration))
		{
			return false;
		}

		info.CollThis = this;
		info.CollOther = &col;

		return true;
	}
	case ColliderType::Sphere:
	{
		Sphere &other = ((SphereCollider&)col).GetSphere();

		if (!Intersection::AABBSphere(aabb, other, info.Point, info.Normal, info.Penetration))
		{
			return false;
		}

		info.CollThis = this;
		info.CollOther = &col;

		return true;
	}
	case ColliderType::Mesh:
	{
		if (!Intersection::MeshAABB(((MeshCollider&)col).GetTriangles(), aabb, info.Point, info.Normal, info.Penetration))
		{
			return false;
		}

		info.CollThis = this;
		info.CollOther = &col;

		return true;
	}
	default:
		ASSERT(0);
	}

	return false;
}