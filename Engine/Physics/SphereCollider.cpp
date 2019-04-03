#include "SphereCollider.h"
#include "MeshCollider.h"
#include "AABBCollider.h"

SphereCollider::SphereCollider()
{ }

SphereCollider::SphereCollider(const Sphere &source) : sphere(source) { }

void SphereCollider::SetTransform(const Transform * t)
{
	this->t = t;
}

Sphere SphereCollider::GetSphere()
{
	return Sphere(sphere.GetCenter() + t->GetPosition(), sphere.GetRadius());
}

const Sphere SphereCollider::GetSphere() const
{
	return Sphere(sphere.GetCenter() + t->GetPosition(), sphere.GetRadius());
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
	return Sphere(sphere.GetCenter() + t->GetPosition(), sphere.GetRadius());
}

PhysicMaterial SphereCollider::GetPhysicMaterial() const
{
	return physicMaterial;
}

bool SphereCollider::Intersect(const ICollider &col, CollisionInfo &info) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();

		if (!Intersection::AABBSphere(other, GetSphere(), info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
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

		if (!Intersection::SphereSphere(GetSphere(), other, info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
		{
			return false;
		}

		info.CollThis = this;
		info.CollOther = &col;

		return true;
	}
	case ColliderType::Mesh:
	{
		if (!Intersection::MeshSphere(((MeshCollider&)col).GetTriangles(), GetSphere(), info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
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