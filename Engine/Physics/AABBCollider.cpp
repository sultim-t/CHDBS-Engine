#include "AABBCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"

bool AABBCollider::Intersect(const ICollider & col) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();
		return Intersection::AABBAABB(aabb, other);
	}
	case ColliderType::Sphere:
	{
		Sphere &other = ((SphereCollider&)col).GetSphere();
		return Intersection::AABBSphere(aabb, other);
	}
	case ColliderType::Mesh:
	{
		const MeshResource &other = ((MeshCollider&)col).GetMesh();
		return Intersection::MeshAABB(other, aabb);
	}
	default:
		ASSERT(0);
	}

	return false;
}
