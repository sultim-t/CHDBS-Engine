#include "MeshCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include <Engine/Components/CMesh.h>

bool MeshCollider::Intersect(const ICollider & col, CollisionInfo &info) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();

		if (!Intersection::MeshAABB(*triangles, other, info.Point, info.Normal))
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

		if (!Intersection::MeshSphere(*triangles, other, info.Point, info.Normal))
		{
			return false;
		}

		info.CollThis = this;
		info.CollOther = &col;

		return true;

	}
	default:
		// other intersections are not implemeted
		ASSERT(0);
	}

	return false;
}