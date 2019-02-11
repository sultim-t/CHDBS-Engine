//#pragma once
//
//#include <vector>
//
//#include "Shader.h"
//#include "Texture.h"
//#include "Material.h"
//#include <Engine\Math\Vector.h>
//
//struct Vertex
//{
//	Vector3 Position;
//	Vector3 Normal;
//	Vector2 TexCoords; // uvs
//	/*Vector3 Tangent;
//	Vector3 Bitangent;*/
//};
//
//class Mesh : i
//{
//private:
//	unsigned vao, vbo, ibo;
//
//public:
//	std::vector<Vertex> Vertices;
//	std::vector<unsigned> Indices;
//
//	Material material;
//
//	// temp
//	std::vector<Texture> Textures;
//
//private:
//	bool FindSimilarVertex(const Vertex &vert, const std::vector<Vertex> &vertices, unsigned &foundIndex);
//
//public:
//	// Empty constructor
//	Mesh();
//	// Construct mesh from std::vector's
//	Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices, const std::vector<Texture> textures = std::vector<Texture>());
//	// Constructor with calculating indices
//	Mesh(const std::vector<Vertex> vertices, const std::vector<Texture> textures = std::vector<Texture>());
//
//	~Mesh();
//
//	// Inits vao, vbo, ibo; positions, normals, uvs
//	void Init();
//
//	void BindMaterial();
//
//	// TEMP function:
//	// Inits mesh from array: {x, y, z, nx, ny, nz, u, v, ...}, count is sizeof(all) 
//	void Init(float *all, int count);
//
//	// Renders mesh with a specific shader
//	void Draw(const Shader &shader) const;
//	// Renders mesh with a specific shader
//	void DrawElements(const Shader &shader) const;
//	// Render mesh to shadowmap
//	void DrawToShadowMap(FramebufferTexture &shadowMap);
//
//	void AddTexture(const Texture &texture);
//
//	const unsigned &GetVAO() const;
//	unsigned GetVertexCount() const;
//};
//
