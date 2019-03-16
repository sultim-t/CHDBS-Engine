#pragma once

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Transform.h>
#include <Engine/Rendering/Bone.h>

// Mesh holder
class MeshResource
{
private:
	StaticArray<Vertex5>	vertices;
	StaticArray<UINT>		indices;

	// Faces. Every face is a triangle
	StaticArray<Triangle>	triangles;
	
	// Bones for current mesh
	StaticArray<Bone>		bones;

	// Original transform -- TODO: delete
	Transform transform;

	String path; // -- TODO: delete?
	
public:
	// Assign 
	inline MeshResource(const char *path, const Transform &transform, const StaticArray<Vertex5> &vertices, const StaticArray<UINT> &indices, const StaticArray<Triangle> &triangles);
	inline MeshResource(const char *path, const StaticArray<Vertex5> &vertices, const StaticArray<UINT> &indices, const StaticArray<Triangle> &triangles, const StaticArray<Bone> &bones);

	inline const StaticArray<Vertex5>	&GetVertices() const;
	inline const StaticArray<UINT>		&GetIndices() const;
	inline const StaticArray<Triangle>	&GetTriangles() const;
	inline const StaticArray<Bone>		&GetBones() const;
	inline const Transform				&GetTransform() const;
	inline const String					&GetPath() const;

	// Clear all data
	inline void Delete();
};

// TODO: delete
inline MeshResource::MeshResource(const char *path, const Transform &transform, const StaticArray<Vertex5> &vertices, const StaticArray<UINT> &indices, const StaticArray<Triangle> &triangles)
{
	this->path = path;
	this->transform = transform;
	this->vertices = vertices;
	this->indices = indices;
	this->triangles = triangles;
}

inline MeshResource::MeshResource(const char *path, const StaticArray<Vertex5> &vertices, const StaticArray<UINT> &indices, const StaticArray<Triangle> &triangles, const StaticArray<Bone> &bones)
{
	this->path = path;
	this->transform = transform;
	this->vertices = vertices;
	this->indices = indices;
	this->triangles = triangles;
	this->bones = bones;
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
	return bones;
}

inline const Transform &MeshResource::GetTransform() const
{
	return transform;
}

inline const String &MeshResource::GetPath() const
{
	return path;
}

inline void MeshResource::Delete()
{
	vertices.Delete();
	indices.Delete();
	triangles.Delete();
}
