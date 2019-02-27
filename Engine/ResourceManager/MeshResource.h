#pragma once

#include <Engine/DataStructures/StaticArray.h>

// Mesh holder
class MeshResource
{
private:
	StaticArray<Vertex5> vertices;
	StaticArray<UINT> indices;
	StaticArray<Triangle> triangles;

	String path;
	
//private:
//	MeshResource(const MeshResource&) = delete;
//	MeshResource(MeshResource&&) = delete;

public:
	// Assign 
	inline MeshResource(const char *path, const StaticArray<Vertex5> &vertices, const StaticArray<UINT> &indices, const StaticArray<Triangle> &triangles);

	inline const StaticArray<Vertex5> &GetVertices() const;
	inline const StaticArray<UINT> &GetIndices() const;
	inline const StaticArray<Triangle> &GetTriangles() const;
	inline const String &GetPath() const;
};

inline MeshResource::MeshResource(const char * path, const StaticArray<Vertex5>& vertices, const StaticArray<UINT>& indices, const StaticArray<Triangle>& triangles)
{
	this->path = path;
	this->vertices = vertices;
	this->indices = indices;
	this->triangles = triangles;
}

inline const StaticArray<Vertex5>& MeshResource::GetVertices() const
{
	return vertices;
}

inline const StaticArray<UINT>& MeshResource::GetIndices() const
{
	return indices;
}

inline const StaticArray<Triangle>& MeshResource::GetTriangles() const
{
	return triangles;
}

inline const String & MeshResource::GetPath() const
{
	return path;
}
