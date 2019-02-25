#pragma once

#include <vector>

#include "IComponent.h"
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/Vertex.h>

// Mesh with material
class Mesh
{
	friend class RenderingSystem;

private:
	MeshID meshId;
	
	UINT vao, vbo, ibo;

	// data
	std::vector<Vertex5> vertices;
	std::vector<UINT> indices;

	// mesh transform only for animations
	Transform transform;
	// material to render this mesh
	Material material;

	// name of this mesh
	String name;
	
public:
	// Construct mesh from std::vector's
	Mesh(const std::vector<Vertex5> vertices, const std::vector<UINT> indices);
	Mesh(const std::vector<Vertex5> vertices, const std::vector<UINT> indices, const Material &material);
	~Mesh();

	// Inits vao, vbo, ibo; positions, normals, uvs
	void Init();
	void BindMaterial(const Material &material);

	// Activates material
	void ActivateMaterial(const Matrix4 &modelTransform) const;
	// Renders mesh
	void Draw() const;

	UINT GetVAO() const;
	UINT GetVertexCount() const;
	const String &GetName() const;

	const Material &GetMaterial() const;
	Material &GetMaterial();

	const Transform &GetTransform() const;
	Transform &GetTransform();

	void SetName(const char *name);
	void SetTransform(const Transform &transform);
};

