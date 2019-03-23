#include "SphereCollider.h"
#include "MeshCollider.h"
#include "AABBCollider.h"

bool SphereCollider::Intersect(const ICollider &col, CollisionInfo &info) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();

		if (!Intersection::AABBSphere(other, GetSphere(), info.Point, info.Normal, info.Penetration))
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

		if (!Intersection::SphereSphere(GetSphere(), other, info.Point, info.Normal, info.Penetration))
		{
			return false;
		}

		info.CollThis = this;
		info.CollOther = &col;

		return true;
	}
	case ColliderType::Mesh:
	{
		if (!Intersection::MeshSphere(((MeshCollider&)col).GetTriangles(), GetSphere(), info.Point, info.Normal, info.Penetration))
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