#pragma once

#include <vector>

#include "IComponent.h"
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Math/Vector.h>

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoords; // uvs
};

struct Vertex5
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoords; // uvs
	Vector3 Tangent;
	Vector3 Bitangent;
};

class Mesh
{
	friend class RenderingSystem;

private:
	MeshID meshId;
	UINT vao, vbo, ibo;

private:
	std::vector<Vertex5> vertices;
	std::vector<UINT> indices;

	Material material;
	
public:
	// Construct mesh from std::vector's
	Mesh(const std::vector<Vertex5> vertices, const std::vector<UINT> indices, const Material &material);
	~Mesh();

	// Inits vao, vbo, ibo; positions, normals, uvs
	void Init();
	void BindMaterial(const Material &material);

	// Renders mesh with current material
	void Draw(const Matrix4 &transformation) const;
	// Render mesh without activating material
	void DrawToShadowMap() const;

	UINT GetVAO() const;
	UINT GetVertexCount() const;
	Material &GetMaterial();
	const Material &GetMaterial() const;
};

