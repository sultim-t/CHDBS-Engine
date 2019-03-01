#include "AABBCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include <Engine/Components/CMesh.h>

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
		return Intersection::MeshAABB(((MeshCollider&)col).GetMesh().GetTriangles(), aabb);
	}
	default:
		ASSERT(0);
	}

	return false;
}
