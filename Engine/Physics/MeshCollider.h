#pragma once
#include "ICollider.h"
#include <Engine/ResourceManager/MeshColliderResource.h>
#include <Engine/DataStructures/StaticArray.h>

// Mesh collider for static colliders
class MeshCollider : public ICollider
{
private:
	// Triangles calculated only once
	StaticArray<Triangle> triangles;
	// Calculated bounding sphere
	// Note: generated only once
	Sphere boundingSphere;
	PhysicMaterial physicMaterial;

private:
	void CalculateBoundingSphere();

public:
	void AddTriangles(const StaticArray<Triangle> &mesh);
	void AddTriangles(const StaticArray<Triangle> &mesh, const Transform &tranformation);

	// Get triangles
	const StaticArray<Triangle> &GetTriangles() const;

	ColliderType GetColliderType() const override;
	bool Intersect(const ICollider &col, CollisionInfo &info) const override;
	Sphere GetBoundingSphere() const override;
	PhysicMaterial GetPhysicMaterial() const override;
};