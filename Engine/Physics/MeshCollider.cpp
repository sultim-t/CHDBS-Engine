#include "MeshCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

bool MeshCollider::Intersect(const ICollider & col) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();
		return Intersection::MeshAABB(*mesh, other);
	}
	case ColliderType::Sphere:
	{
		Vector3 point;
		Sphere &other = ((SphereCollider&)col).GetSphere();
		return Intersection::MeshSphere(*mesh, other, point);
	}
	default:
		// other intersections are not implemeted
		ASSERT(0);
	}

	return false;
}