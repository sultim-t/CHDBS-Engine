#pragma once
#include "MeshResource.h"
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Transform.h>
#include <Engine/Math/Triangle.h>

class MeshColliderResource
{
private:
	StaticArray<Triangle> triangles;

public:
	// Assigns
	inline MeshColliderResource(const StaticArray<Triangle> &triangles);
	// Copies triangle array from mesh
	inline MeshColliderResource(const MeshResource &mesh);

	// Creates mesh collider with specified transformation
	// Note: allocates copy of fields
	inline MeshColliderResource TransformCollider(const Transform &t) const;

	// Clear all data
	inline void Delete();
};

inline MeshColliderResource::MeshColliderResource(const StaticArray<Triangle>& triangles)
{
	this->triangles = triangles;
}

inline MeshColliderResource::MeshColliderResource(const MeshResource &mesh)
{
	this->triangles = mesh.GetTriangles().GetCopy();
}

inline MeshColliderResource MeshColliderResource::TransformCollider(const Transform &t) const
{
	Matrix4 transform = t.GetTransformMatrix();

	StaticArray<Triangle> newTriangles = StaticArray<Triangle>();
	newTriangles.Init(triangles.GetSize());

	for (UINT i = 0; i < triangles.GetSize(); i++)
	{
		newTriangles[i].A = transform * Vector4(triangles[i].A);
		newTriangles[i].B = transform * Vector4(triangles[i].B);
		newTriangles[i].C = transform * Vector4(triangles[i].C);
	}

	return MeshColliderResource(newTriangles);
}

inline void MeshColliderResource::Delete()
{
	triangles.Delete();
}
