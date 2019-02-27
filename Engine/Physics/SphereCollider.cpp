#include "SphereCollider.h"
#include "MeshCollider.h"
#include "AABBCollider.h"

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
		const MeshResource &other = ((MeshCollider&)col).GetMesh();
		return Intersection::MeshSphere(other, sphere, point);
	}
	default:
		ASSERT(0);
	}

	return false;
}