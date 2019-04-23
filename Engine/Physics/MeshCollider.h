#pragma once
#include "ICollider.h"
#include <Engine/ResourceManager/MeshColliderResource.h>
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Physics/MeshBoundingPart.h>

#define MESH_COLLIDER_PART_SIZE 100.0f

// Mesh collider for static colliders
class MeshCollider : public ICollider
{
private:
	// Triangles calculated only once
	StaticArray<Triangle> triangles;

	// Calculated bounding shapes
	// Note: generated only when new triangles are added 
	AABB	boundingBox;
	Sphere	boundingSphere;

	StaticArray<MeshBoundingPart> parts;
	UINT partsCount;

	PhysicMaterial physicMaterial;

protected:
	void RecalculateBoundingBox();
	void RecalculateBoundingSphere();
	void RecalculateBoundingParts();
	// Destroy all parts in array
	void DeleteParts();

public:
	// Destuctor
	~MeshCollider();

	void AddTriangles(const StaticArray<Triangle> &mesh);
	void AddTriangles(const StaticArray<Triangle> &mesh, const Transform &tranformation);

	// Get triangles
	const StaticArray<Triangle> &GetTriangles() const;

public:
	ColliderType	GetColliderType() const override;
	bool			Intersect(const ICollider &col, CollisionInfo &info) const override;
	Sphere			GetBoundingSphere() const override;
	PhysicMaterial	GetPhysicMaterial() const override;
};