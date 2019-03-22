//#pragma once
//
//#include "IComponent.h"
//#include <Engine/DataStructures/StaticArray.h>
//#include <Engine/Rendering/Material.h>
//#include <Engine/Rendering/Texture.h>
//#include <Engine/Rendering/Vertex.h>
//#include <Engine/ResourceManager/MeshResource.h>
//
//// Mesh with material
//class Mesh
//{
//	friend class RenderingSystem;
//
//private:
//	MeshID meshId;
//	UINT vao, vbo, ibo;
//
//	// mesh transform only for animations
//	Transform transform;
//	// material to render this mesh
//	Material material;
//	
//	// pointer to mesh resource to access mesh data
//	// no read/write access
//	const MeshResource *resource;
//
//	String name;
//	
//public:
//	// Construct mesh from mesh resource
//	Mesh(const MeshResource *resource);
//	Mesh(const MeshResource *resource, const Material &material);
//	~Mesh();
//
//	// Inits vao, vbo, ibo; positions, normals, uvs
//	void Init();
//	void BindMaterial(const Material &material);
//
//	// Set material variables
//	void PrepareMaterial(const Matrix4 &modelTransform) const;
//	// Activates material
//	void ActivateMaterial() const;
//	// Renders mesh
//	void Draw() const;
//
//	UINT				GetVertexCount() const;
//	const String		&GetName() const;
//	const Material		&GetMaterial() const;
//	Material			&GetMaterial();
//	const Transform		&GetTransform() const;
//	Transform			&GetTransform();
//	const MeshResource &GetMeshResource() const;
//
//	void SetName(const char *name);
//	void SetTransform(const Transform &transform);
//};
//
