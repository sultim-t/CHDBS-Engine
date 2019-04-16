#pragma once
#include "ISystem.h"

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/DataStructures/LinkedList.h>
#include <Engine/DataStructures/HashTable.h>

#include <Engine/Components/Mesh.h>
#include <Engine/Components/CLight.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/FramebufferTexture.h>

class RenderingSystem : public ISystem
{
private:
	HashTable<MeshID	, Mesh*		>	meshes;
	HashTable<MaterialID, Material*	>	materials;
	HashTable<TextureID	, ITexture*	>	textures;
	HashTable<ShaderID	, Shader*	>	shaders;

	// Stores array of arrays with same models (Only static),
	// therefore same meshes, same material
	DynamicArray<DynamicArray<CModel*>>	batchedModels;
	
	// Stores all textures used in this material
	HashTable<MaterialID, LinkedList<ITexture*>*>	matTextures;
	
	// Shader used in material
	HashTable<MaterialID, ShaderID			>		matShaders;

	// Sorted camera list
	// First element is first to render
	DynamicArray<CCamera*>			cameras;
	DynamicArray<CLight*>			lights;
	DynamicArray<Shader*>			allShaders;
	DynamicArray<CModel*>			allModels;
	DynamicArray<CParticleSystem*>	particleSystems;

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
	~RenderingSystem();

	RenderingSystem(RenderingSystem&) = delete;
	RenderingSystem(RenderingSystem&&) = delete;
	RenderingSystem &operator=(const RenderingSystem&) = delete;

	void DrawMesh(UINT vao, UINT indicesCount);

	// Generate shadowmap to framebuffer, according to camera frustum to fit entire shadowmap
	void CreateShadowMap(const CLight &light, const Frustum &frustum, FramebufferTexture &shadowMap);

public:
	// Init structures
	void Init() override;
	// Render frame
	void Update() override;

	// Get instance of system
	static RenderingSystem &Instance();

	// Register material by calculating its ID
	// and saving the pointer
	void Register(Material *material);
	// Register texture by calculating its ID
	// and saving the pointer
	void Register(ITexture *texture);
	// Register shader by calculating its ID
	// and saving the pointer
	void Register(Shader *shader);
	// Attach mesh to material
	void Register(Mesh *mesh, const Material &material);
	// Register model by saving its pointer
	void Register(CModel *model);
	// Register light by saving its pointer
	void Register(CLight *light);
	// Register camera by saving its pointer
	void Register(CCamera *camera);
	// Register particle system by saving its pointer
	void Register(CParticleSystem *ps);


	// Get mesh by its ID
	Mesh *GetMesh(MeshID id) const;
	// Get material by its ID
	Material *GetMaterial(MaterialID id) const;
	// Get texture by its ID
	ITexture *GetTexture(TextureID id) const;
	// Get mesh by its ID
	Shader *GetShader(ShaderID id) const;
	// Get list of IDs of meshes with same material
	// const LinkedList<Mesh*> *GetMatMesh(MaterialID id) const;
	// Get list of IDs of meshes with same material
	// inline LinkedList<Mesh*> *GetMatMesh(MaterialID id);
};