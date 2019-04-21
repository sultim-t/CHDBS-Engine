#include "MeshCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

const StaticArray<Triangle> &MeshCollider::GetTriangles() const
{
	return triangles;
}

ColliderType MeshCollider::GetColliderType() const
{
	return ColliderType::Mesh;
}

Sphere MeshCollider::GetBoundingSphere() const
{
	return boundingSphere;
}

PhysicMaterial MeshCollider::GetPhysicMaterial() const
{
	return physicMaterial;
}

bool MeshCollider::Intersect(const ICollider & col, CollisionInfo &info) const
{
	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		AABB &other = ((AABBCollider&)col).GetAABB();

		if (!Intersection::MeshAABB(triangles, other, info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
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

		if (!Intersection::MeshSphere(triangles, other, info.Contact.Point, info.Contact.Normal, info.Contact.Penetration))
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

void MeshCollider::CalculateBoundingSphere()
{
	UINT size = triangles.GetSize();

	Vector3 min = Vector3(0.0f);
	Vector3 max = Vector3(0.0f);

	// for each triangle
	for (UINT t = 0; t < size; t++)
	{
		const Triangle &triangle = triangles[t];

		// for each vertex in triangle
		for (int i = 0; i < 3; i++)
		{
			const Vector3 &vertex = triangle[i];

			// for each dimension in vector
			for (int j = 0; j < 3; j++)
			{
				float d = vertex[j];

				if (d < min[j])
				{
					min[j] = d;
				}

				if (d > max[j])
				{
					max[j] = d;
				}
			}
		}
	}

	Vector3 center = (max + min) * 0.5f;
	float radius = (max - min).Length() * 0.5f;

	boundingSphere = Sphere(center, radius);
}

void MeshCollider::AddTriangles(const StaticArray<Triangle> &mesh)
{
	AddTriangles(mesh, Transform());
}

void MeshCollider::AddTriangles(const StaticArray<Triangle> &mesh, const Transform &t)
{
	UINT curSize = triangles.GetSize();
	UINT addingSize = mesh.GetSize();
	UINT newSize = curSize + addingSize;

	// save old data
	StaticArray<Triangle> &oldData = triangles.GetCopy();

	// delete old array
	triangles.Delete();
	// reinit with new size
	triangles.Init(newSize);

	// copy old data to new array
	for (UINT i = 0; i < curSize; i++)
	{
		triangles[i].A = oldData[i].A;
		triangles[i].B = oldData[i].B;
		triangles[i].C = oldData[i].C;
	}

	// for each new triangle: add it transformed
	for (UINT i = curSize; i < newSize; i++)
	{
		const Triangle &cur = mesh[i - curSize];

		// transform
		triangles[i].A = t.PointFromLocal(cur.A);
		triangles[i].B = t.PointFromLocal(cur.B);
		triangles[i].C = t.PointFromLocal(cur.C);
	}

	// recalculate
	CalculateBoundingSphere();
}
