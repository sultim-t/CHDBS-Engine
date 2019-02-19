#include "CMesh.h"
#include <Engine/Rendering/OpenGL.h>
#include <vector>

Mesh::Mesh(const std::vector<Vertex5> vertices, const std::vector<UINT> indices, const Material &material)
{
	this->vertices = vertices;
	this->indices = indices;
	this->material = material;

	Init();
}

Mesh::~Mesh()
{ }

void Mesh::Init()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	// load to vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex5), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set attributes
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)offsetof(Vertex5, Normal));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)offsetof(Vertex5, TexCoords));
	// tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)offsetof(Vertex5, Tangent));
	// bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex5), (void*)offsetof(Vertex5, Bitangent));

	glBindVertexArray(0);
}

void Mesh::BindMaterial(const Material &material)
{
	this->material = material;
}

void Mesh::ActivateMaterial(const Matrix4 &transformation) const
{
	material.BindModelMatrix(transformation);
	material.Activate();
}

void Mesh::Draw() const
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

UINT Mesh::GetVAO() const
{
	return vao;
}

UINT Mesh::GetVertexCount() const
{
	return vertices.size();
}

Material &Mesh::GetMaterial()
{
	return material;
}

const Material &Mesh::GetMaterial() const
{
	return material;
}
