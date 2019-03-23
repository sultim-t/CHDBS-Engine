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
	// Note: generated only once
	Sphere boundingSphere;

private:
	void CalculateBoundingSphere();

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

inline MeshCollider::MeshCollider() : triangles(nullptr) { }

inline MeshCollider::MeshCollider(const MeshColliderResource *mesh)
{
	this->triangles = &mesh->GetTriangles();
	CalculateBoundingSphere();
}

inline MeshCollider::MeshCollider(const StaticArray<Triangle> *triangles)
{
	this->triangles = triangles;
	CalculateBoundingSphere();
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