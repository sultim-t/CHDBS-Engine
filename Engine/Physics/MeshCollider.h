#pragma once
#include "ICollider.h"
#include <Engine/ResourceManager/MeshColliderResource.h>
#include <Engine/DataStructures/StaticArray.h>

// Mesh collider for static colliders
class MeshCollider : public ICollider
{
private:
	// Triangles doesn't change
	const StaticArray<Triangle> *triangles;
	// Calculated bounding sphere
	Sphere boundingSphere;

private:
	inline void CalculateBoundingSphere();

public:
	// Empty constructor
	inline MeshCollider();
	// Save pointer to mesh collider's triangles
	inline MeshCollider(const MeshColliderResource *mesh);
	// Savee pointer to array of triangles
	inline MeshCollider(const StaticArray<Triangle> *triangles);

	// Get triangles
	inline const StaticArray<Triangle> &GetTriangles() const;

	inline ColliderType GetColliderType() const override;
	bool Intersect(const ICollider &col, CollisionInfo &info) const override;
	inline Sphere GetBoundingSphere() const override;
};

inline void MeshCollider::CalculateBoundingSphere()
{
	UINT size = triangles->GetSize();

	Vector3 min = Vector3(0.0f); 
	Vector3 max = Vector3(0.0f);

	for (UINT t = 0; t < size; t++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				float d = triangles->operator[](t)[i][j];
				
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

inline MeshCollider::MeshCollider()
{
	this->triangles = nullptr;
}

inline MeshCollider::MeshCollider(const MeshColliderResource *mesh)
{
	this->triangles = &mesh->GetTriangles();
}

inline MeshCollider::MeshCollider(const StaticArray<Triangle> *triangles)
{
	this->triangles = triangles;
}

inline const StaticArray<Triangle> &MeshCollider::GetTriangles() const
{
	return *triangles;
}

inline ColliderType MeshCollider::GetColliderType() const
{
	return ColliderType::Mesh;
}

inline Sphere MeshCollider::GetBoundingSphere() const
{
	return boundingSphere;
}