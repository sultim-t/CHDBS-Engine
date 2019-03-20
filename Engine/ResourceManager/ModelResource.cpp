#include "ModelResource.h"
#include "MeshResource.h"
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/Vertex.h>

void ModelResource::InitMesh(const MeshResource *resource, UINT &vao, UINT &vbo, UINT &ibo, bool useTan, bool dynamic)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	// load to vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	UINT size = useTan ? sizeof(Vertex5) : sizeof(Vertex);

	if (dynamic)
	{
		glBufferData(GL_ARRAY_BUFFER, resource->GetVertices().GetSize() * size, resource->GetVertices().GetArray(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, resource->GetVertices().GetSize() * size, resource->GetVertices().GetArray(), GL_DYNAMIC_DRAW);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	// indices must not be changed
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, resource->GetIndices().GetSize() * sizeof(unsigned int), resource->GetIndices().GetArray(), GL_STATIC_DRAW);

	// set attributes
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	if (useTan)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, Normal));
	}
	else
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex, Normal));
	}

	// texture coords
	glEnableVertexAttribArray(2);
	if (useTan)
	{
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, TexCoords));
	}
	else
	{
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex, TexCoords));
	}

	if (useTan)
	{
		// tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, Tangent));
		// bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, Bitangent));
	}

	glBindVertexArray(0);
}

inline void ModelResource::InitStatic()
{
	for (UINT i = 0; i < hierarchy->GetMeshes().GetSize(); i++)
	{
		// init for rendering
		InitMesh(hierarchy->GetMeshes()[i], vaos[i], vbos[i], ibos[i], true, false);
	}
}

inline void ModelResource::InitDynamic()
{
	for (UINT i = 0; i < hierarchy->GetMeshes().GetSize(); i++)
	{
		// init for rendering
		InitMesh(hierarchy->GetMeshes()[i], vaos[i], vbos[i], ibos[i], true, true);
	}
}