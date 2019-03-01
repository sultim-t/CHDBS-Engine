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
	// Note:  allocates copy of fields
	inline void TransformCollider(const Transform &t, StaticArray<Triangle> outTriangles) const;

	inline const StaticArray<Triangle> &GetTriangles() const;

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

inline void MeshColliderResource::TransformCollider(const Transform &t, StaticArray<Triangle> outTriangles) const
{
	Matrix4 transform = t.GetTransformMatrix();

	UINT size = triangles.GetSize();
	for (UINT i = 0; i < size; i++)
	{
		outTriangles[i].A = Vector3(transform * Vector4(triangles[i].A));
		outTriangles[i].B = Vector3(transform * Vector4(triangles[i].B));
		outTriangles[i].C = Vector3(transform * Vector4(triangles[i].C));
	}
}

inline const StaticArray<Triangle>& MeshColliderResource::GetTriangles() const
{
	return triangles;
}

inline void MeshColliderResource::Delete()
{
	triangles.Delete();
}
