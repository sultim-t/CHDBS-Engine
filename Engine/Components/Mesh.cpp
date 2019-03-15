#include "Mesh.h"
#include <Engine/Rendering/OpenGL.h>


Mesh::Mesh(const MeshResource * resource)
{
	this->resource = resource;
	Init();
}

Mesh::Mesh(const MeshResource * resource, const Material & material)
{
	this->resource = resource;
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
	glBufferData(GL_ARRAY_BUFFER, resource->GetVertices().GetSize() * sizeof(Vertex5), resource->GetVertices().GetArray(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, resource->GetIndices().GetSize() * sizeof(unsigned int), resource->GetIndices().GetArray(), GL_STATIC_DRAW);

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

void Mesh::PrepareMaterial(const Matrix4 & modelTransform) const
{
	Matrix4 &globalMeshTransform = transform.GetTransformMatrix() * modelTransform;
	material.BindModelMatrix(globalMeshTransform);
}

void Mesh::ActivateMaterial() const
{
	material.Activate();
}

void Mesh::Draw() const
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, resource->GetIndices().GetSize(), GL_UNSIGNED_INT, 0);
}


UINT Mesh::GetVertexCount() const
{
	return resource->GetVertices().GetSize();
}

Material &Mesh::GetMaterial()
{
	return material;
}

const Transform & Mesh::GetTransform() const
{
	return transform;
}

Transform & Mesh::GetTransform()
{
	return transform;
}

const MeshResource & Mesh::GetMeshResource() const
{
	return *resource;
}

const Material &Mesh::GetMaterial() const
{
	return material;
}

const String & Mesh::GetName() const
{
	return name;
}

void Mesh::SetTransform(const Transform &transform)
{
	this->transform = transform;
}

void Mesh::SetName(const char * name)
{
	this->name = name;
}
