#pragma once

#include "MeshResource.h"
#include "TextureResource.h"
#include "ModelResource.h"
#include "MeshColliderResource.h"

#include <Engine/DataStructures/HashTable.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Rendering/ModelHierarchy.h>

class ResourceManager
{
private:	
	// Stores all loaded meshes
	DynamicArray<MeshResource*>			meshResources;
	DynamicArray<MeshColliderResource*> meshColliderResources;

	// Stores all loaded textures
	HashTable<String, TextureResource*> textureResources;
	// Stores all loaded models
	HashTable<String, ModelHierarchy*>	models;

	// TODO: delete
	HashTable<String, ModelResource*>	modelResources;

private:
	// Process node
	void ProcessModelNode(void *n, const void *s, ModelResource *outModel);
	// Loads data from mesh
	MeshResource *ProcessMesh(void *m, const void *s, const char *name, const Transform &transform);
	// std::vector<ITexture> LoadMaterialTextures(void * m, int t, TextureType myType, CModel &outModel);
	
public:
	// Deallocates allocated memory
	~ResourceManager();
	
	// Init data structures
	void Init();
	// Delete loaded resources
	void Unload();

	const TextureResource	*LoadTexture(char const *path);
	const ModelHierarchy	*LoadModelF(const char *path);

	// TODO: delete
	const ModelResource		*LoadModel(const char *path);

	static ResourceManager &Instance();
};