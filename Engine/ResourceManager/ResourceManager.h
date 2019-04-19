#pragma once

#include "MeshResource.h"
#include "TextureResource.h"
#include "ModelResource.h"
#include "MeshColliderResource.h"
#include "SceneResource.h"
#include "ShaderResource.h"
#include "EntityResource.h"

#include <Engine/DataStructures/HashTable.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Rendering/ModelHierarchy.h>

class ResourceManager
{
private:	
	// Stores all loaded meshes
	// DynamicArray<MeshResource*>			meshResources;

	// Stores all loaded mesh colliders
	DynamicArray<MeshColliderResource*> meshColliderResources;

	// Stores all loaded textures
	HashTable<String, TextureResource*> textureResources;

	// Stores all loaded models
	HashTable<String, ModelResource*>	modelResources;

	// Stores all loaded scenes
	HashTable<String, SceneResource*>	sceneResources;

	// Stores all loaded shaders
	HashTable<String, ShaderResource*>	shaderResources;

	// Stores all loaded entities
	HashTable<String, EntityResource*>	entityResources;

private:	
	// Allocates mesh resource and copies data
	void CopyMesh(void *from, MeshResource *to);
	// Allocates animation and copies data
	void CopyAnimation(void *from, Animation *to);
	// Copies bones data
	void CopyBones(void *from, ModelHierarchy *hierarchy, MeshResource *to);
	// Allocates model nodes and copies data
	ModelNode *ProcessModelNode(void *n, const void *s, ModelNode *parent);

public:
	// Deallocates allocated memory
	~ResourceManager();
	
	// Init data structures
	void Init();
	// Delete loaded resources
	void Unload();

	const TextureResource	*LoadTexture(char const *path);
	const ModelResource		*LoadModel(const char *path);
	const SceneResource		*LoadScene(const char *path);
	const ShaderResource	*LoadShader(const char *vertexPath, const char *fragmentPath);
	const EntityResource	*LoadEnitity(const char *path);

	static ResourceManager &Instance();
};