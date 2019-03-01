#include "SphereCollider.h"
#include "MeshCollider.h"
#include "AABBCollider.h"
#include <Engine/Components/CMesh.h>

bool SphereCollider::Intersect(const ICollider &col) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();
		return Intersection::AABBSphere(other, sphere);
	}
	case ColliderType::Sphere:
	{
		Sphere &other = ((SphereCollider&)col).GetSphere();
		return Intersection::SphereSphere(sphere, other);
	}
	case ColliderType::Mesh:
	{
		Vector3 point;
		return Intersection::MeshSphere(((MeshCollider&)col).GetMesh().GetTriangles(), sphere, point);
	}
	default:
		ASSERT(0);
	}

	return false;
}