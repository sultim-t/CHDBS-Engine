#include "SphereCollider.h"
#include "MeshCollider.h"
#include "AABBCollider.h"
#include <Engine/Math/Intersection.h>

SphereCollider::SphereCollider() { }

SphereCollider::SphereCollider(const Sphere &source) 
	: sphere(source) { }


Sphere SphereCollider::GetSphere() const
{
	return Sphere(sphere.GetCenter() + GetTransform().GetPosition(), sphere.GetRadius());
}

Sphere & SphereCollider::GetSphereRef()
{
	return sphere;
}

ColliderType SphereCollider::GetColliderType() const
{
	return ColliderType::Sphere;
}

Sphere SphereCollider::GetBoundingSphere() const
{
	return GetSphere();
}

bool SphereCollider::Intersect(const ICollider &col, CollisionInfo &info) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();

		Vector3 point, normal;
		float penetration;

		if (!Intersection::SphereAABB(GetSphere(), other, point, normal, penetration))
		{
			return false;
		}

		// only one contact
		info.AddContact(point, normal, penetration);

		return true;
	}
	case ColliderType::Sphere:
	{
		Sphere &other = ((SphereCollider&)col).GetSphere();
		
		Vector3 point, normal;
		float penetration;

		if (!Intersection::SphereSphere(GetSphere(), other, point, normal, penetration))
		{
			return false;
		}

		// only one contact
		info.AddContact(point, normal, penetration);

		return true;
	}
	case ColliderType::Mesh:
	{
		// process in mesh collider with optimzations
		return ((MeshCollider&)col).Intersect(*this, info);
	}
	default:
		ASSERT(0);
	}

	return false;
}