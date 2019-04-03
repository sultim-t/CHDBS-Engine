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
	PhysicMaterial physicMaterial;

private:
	void CalculateBoundingSphere();

public:
	// Empty constructor
	MeshCollider();
	// Save pointer to mesh collider's triangles
	MeshCollider(const MeshColliderResource *mesh);
	// Savee pointer to array of triangles
	MeshCollider(const StaticArray<Triangle> *triangles);

	// Get triangles
	const StaticArray<Triangle> &GetTriangles() const;

	ColliderType GetColliderType() const override;
	bool Intersect(const ICollider &col, CollisionInfo &info) const override;
	Sphere GetBoundingSphere() const override;
	PhysicMaterial GetPhysicMaterial() const override;
};