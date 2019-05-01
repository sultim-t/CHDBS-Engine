#pragma once

#include <Engine/DataStructures/StaticArray.h>
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
	
	// Hierarchy of bones, which is attached to this mesh
	Skeleton				*skeleton;

	// Material to use
	// All materials are stored in model resource
	UINT					materialIndex;

	// Bounding sphere for rendering
	Sphere					boundingSphere;
	bool					boundingCalculated;
	
public:
	// Init arrays
	MeshResource(int verticesSize, int indicesSize, int trianglesSize, int bonesSize, UINT materialIndex);
	~MeshResource();

	const StaticArray<Vertex5>	&GetVertices() const;
	const StaticArray<UINT>		&GetIndices() const;
	const StaticArray<Triangle>	&GetTriangles() const;
	// const StaticArray<Bone*>	&GetBones() const;
	const Skeleton				&GetSkeleton() const;
	UINT						GetMaterialIndex() const;

	// Does this mesh have bones?
	bool HasBones() const;
	// Find bone by name
	// const Bone *GetBone(const char *name) const;

	// Bounding sphere of this mesh
	const Sphere &GetBoundingSphere();

	// Clear all data
	void Delete();
};