#include "ArrayMesh.h"
#include "OpenGL.h"

ArrayMesh::ArrayMesh(const std::vector<Vertex> &vertices)
{
	this->vertices = vertices;
}

void ArrayMesh::Init()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	// load to vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// set attributes
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void ArrayMesh::Bind() const
{
	glBindVertexArray(vao);
}

void ArrayMesh::DrawArrays() const
{
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
