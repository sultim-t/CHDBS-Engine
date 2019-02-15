#pragma once
#include "Vertex.h"
#include <vector>

class ArrayMesh
{
private:
	MeshID meshId;
	UINT vao, vbo;
	std::vector<Vertex> vertices;
	
public:
	ArrayMesh(const std::vector<Vertex> &vertices);

	// Generate buffers
	void Init();
	// Bind VAO
	void Bind() const;
	// Render mesh
	void DrawArrays() const;
};