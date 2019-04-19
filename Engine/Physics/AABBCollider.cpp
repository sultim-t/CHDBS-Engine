#include "AABBCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"

AABBCollider::AABBCollider()
{ }

AABBCollider::AABBCollider(const AABB &aabb)
{
	this->aabb = aabb;
}

void AABBCollider::SetTransform(const Transform *t)
{
	this->t = t;
}

AABB AABBCollider::GetAABB() const
{
	AABB result = AABB(aabb);
	result.Move(t->GetPosition());

	return result;
}

AABB &AABBCollider::GetAABBRef()
{
	return aabb;
}

ColliderType AABBCollider::GetColliderType() const
{
	return ColliderType::AABB;
}

Sphere AABBCollider::GetBoundingSphere() const
{
	Vector3 center;
	float radius;

	center = aabb.GetCenter() + t->GetPosition();
	radius = aabb.GetSize().Length() * 0.5f;

	return Sphere(center, radius);
}

PhysicMaterial AABBCollider::GetPhysicMaterial() const
{
	return physicMaterial;
}

bool AABBCollider::Intersect(const ICollider & col, CollisionInfo &info) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();

		if (!Intersection::AABBAABB(GetAABB(), other, info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
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

		if (!Intersection::AABBSphere(GetAABB(), other, info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
		{
			return false;
		}

		info.CollThis = this;
		info.CollOther = &col;

		return true;
	}
	case ColliderType::Mesh:
	{
		if (!Intersection::MeshAABB(((MeshCollider&)col).GetTriangles(), GetAABB(), info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
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