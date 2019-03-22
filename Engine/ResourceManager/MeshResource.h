#pragma once

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Rendering/Bone.h>
#include <Engine/Rendering/Vertex.h>
#include <Engine/Math/Triangle.h>

// Mesh holder
class MeshResource
{
	// Memory is deallocated there where it was allocated.
	// Resource manager just loads info to this class
	// but doesn't allocates current StaticArrays.
	friend class ResourceManager;

private:
	StaticArray<Vertex5>	vertices;
	StaticArray<UINT>		indices;

	// Faces. Every face is a triangle
	StaticArray<Triangle>	triangles;
	
	// Bones for current mesh
	StaticArray<Bone>		bones;
	bool hasBones;
	
public:
	// Init arrays
	inline MeshResource(int verticesSize, int indicesSize, int trianglesSize, int bonesSize);

	inline const StaticArray<Vertex5>	&GetVertices() const;
	inline const StaticArray<UINT>		&GetIndices() const;
	inline const StaticArray<Triangle>	&GetTriangles() const;
	inline const StaticArray<Bone>		&GetBones() const;

	inline bool HasBones() const;

	// Clear all data
	inline void Delete();
};

inline MeshResource::MeshResource(int verticesSize, int indicesSize, int trianglesSize, int bonesSize)
{
	ASSERT(verticesSize >= 0 && indicesSize >= 0 && trianglesSize >= 0 && bonesSize >= 0);

	vertices.Init(verticesSize);
	indices.Init(indicesSize);
	triangles.Init(trianglesSize);

	hasBones = bonesSize != 0;

	if (hasBones)
	{
		bones.Init(bonesSize);
	}
}

inline const StaticArray<Vertex5> &MeshResource::GetVertices() const
{
	return vertices;
}

inline const StaticArray<UINT> &MeshResource::GetIndices() const
{
	return indices;
}

inline const StaticArray<Triangle> &MeshResource::GetTriangles() const
{
	return triangles;
}

inline const StaticArray<Bone> &MeshResource::GetBones() const
{
	ASSERT(hasBones);
	return bones;
}

inline bool MeshResource::HasBones() const
{
	return hasBones;
}

inline void MeshResource::Delete()
{
	vertices.Delete();
	indices.Delete();
	triangles.Delete();
}
