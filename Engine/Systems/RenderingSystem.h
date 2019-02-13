#pragma once
#include "ISystem.h"

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/DataStructures/LinkedList.h>
#include <Engine/DataStructures/HashTable.h>

#include <Engine/Components/CMesh.h>
#include <Engine/Components/CLight.h>
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/FramebufferTexture.h>

class RenderingSystem : public ISystem
{
private:
	HashTable<MeshID	, Mesh*		>	meshes;
	HashTable<MaterialID, Material*	>	materials;
	HashTable<TextureID	, Texture*	>	textures;
	HashTable<ShaderID	, Shader*	>	shaders;

	// Stores all meshes that have same material
	HashTable<MaterialID, LinkedList<Mesh*>*>		matMeshes;
	// Stores all textures used in this material
	HashTable<MaterialID, LinkedList<Texture*>*>	matTextures;
	// Shader used in material
	HashTable<MaterialID, ShaderID			>		matShaders;

	// Sorted camera list
	// First element is first to render
	LinkedList<CCamera*>	cameras;
	LinkedList<CLight*>		lights;
	LinkedList<Shader*>		allShaders;
	LinkedList<CModel*>		allModels;

	UINT lastMeshId;
	UINT lastMaterialId;
	UINT lastTextureId;
	UINT lastShaderId;

	// main shadowmap
	FramebufferTexture shadowMap;
	// shader for shadowmapping
	Shader depthShader;

private:
	RenderingSystem();
	RenderingSystem(const RenderingSystem&) = delete;
	RenderingSystem(const RenderingSystem&&) = delete;
	~RenderingSystem();

public:
	// Init structures
	void Init() override;
	// Render frame
	void Update() override;

	void CreateShadowMap(const CLight &light, FramebufferTexture & shadowMap);

	// Get instance of system
	static RenderingSystem &Instance();

	// Register mesh by calculating its ID
	void Register(Mesh *mesh);
	// Register material by calculating its ID
	void Register(Material *material);
	// Register texture by calculating its ID
	void Register(Texture *texture);
	// Register shader by calculating its ID
	void Register(Shader *shader);
	// Attach mesh to material
	void Register(Mesh *mesh, const Material &material);
	// Register model
	void Register(CModel *model);
	// Register light
	void Register(CLight *light);
	// Register camera
	void Register(CCamera *camera);


	// Get mesh by its ID
	Mesh *GetMesh(MeshID id) const;
	// Get material by its ID
	Material *GetMaterial(MaterialID id) const;
	// Get texture by its ID
	Texture *GetTexture(TextureID id) const;
	// Get mesh by its ID
	Shader *GetShader(ShaderID id) const;
	// Get list of IDs of meshes with same material
	const LinkedList<Mesh*> *GetMatMesh(MaterialID id) const;
	// Get list of IDs of meshes with same material
	inline LinkedList<Mesh*> *GetMatMesh(MaterialID id);
};