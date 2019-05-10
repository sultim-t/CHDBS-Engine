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
	// Copies triangle array
	inline MeshColliderResource(const StaticArray<Triangle> &triangles);
	// Copies triangle array from mesh
	inline MeshColliderResource(const MeshResource &mesh);

	// Realculates triangles with specified transformation into "outTriangles"
	// Note: memory must be allocated for "outTriangles"
	inline void TransformCollider(const Transform &t, StaticArray<Triangle> &outTriangles) const;

	inline const StaticArray<Triangle> &GetTriangles() const;

	// Clear all data
	inline void Delete();
};

inline MeshColliderResource::MeshColliderResource(const StaticArray<Triangle>& triangles)
{
	this->triangles.Init(triangles.GetSize());
	this->triangles.CopyFrom(triangles);
}

inline MeshColliderResource::MeshColliderResource(const MeshResource &mesh)
	: MeshColliderResource(mesh.GetTriangles()) { }

inline void MeshColliderResource::TransformCollider(const Transform &t, StaticArray<Triangle> &outTriangles) const
{
	UINT size = triangles.GetSize();
	for (UINT i = 0; i < size; i++)
	{
		outTriangles[i].A = t.PointFromLocal(triangles[i].A);
		outTriangles[i].B = t.PointFromLocal(triangles[i].B);
		outTriangles[i].C = t.PointFromLocal(triangles[i].C);
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
