#pragma once

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Rendering/Bone.h>
#include <Engine/Rendering/Vertex.h>
#include <Engine/Math/Triangle.h>
#include <Engine/Math/Sphere.h>

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
	bool	hasBones;

	// Bounding sphere for rendering
	Sphere	boundingSphere;
	bool	boundingCalculated;
	
public:
	// Init arrays
	inline MeshResource(int verticesSize, int indicesSize, int trianglesSize, int bonesSize);
	inline ~MeshResource();

	inline const StaticArray<Vertex5>	&GetVertices() const;
	inline const StaticArray<UINT>		&GetIndices() const;
	inline const StaticArray<Triangle>	&GetTriangles() const;
	inline const StaticArray<Bone>		&GetBones() const;

	inline bool HasBones() const;

	inline const Sphere &GetBoundingSphere();

	// Clear all data
	inline void Delete();
};

inline MeshResource::MeshResource(int verticesSize, int indicesSize, int trianglesSize, int bonesSize) : boundingCalculated(false)
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

inline MeshResource::~MeshResource()
{
	Delete();
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

inline const Sphere &MeshResource::GetBoundingSphere()
{
	if (boundingCalculated)
	{
		return boundingSphere;
	}

	Vector3 min = Vector3(0.0f);
	Vector3 max = Vector3(0.0f);

	UINT size = vertices.GetSize();
	for (UINT i = 0; i < size; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			float t = vertices[i].Position[k];

			if (t < min[k])
			{
				min[k] = t;
			}
			else if (t > max[k])
			{
				max[k] = t;
			}
		}
	}
	
	Vector3 center = (max + min) * 0.5f;
	float radius = (max - min).Length() * 0.5f;

	boundingSphere = Sphere(center, radius);
	boundingCalculated = true;

	return boundingSphere;
}

inline void MeshResource::Delete()
{
	vertices.Delete();
	indices.Delete();
	triangles.Delete();
}
