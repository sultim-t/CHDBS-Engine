#pragma once

#include "MeshResource.h"
#include "TextureResource.h"
#include "ModelResource.h"
#include "MeshColliderResource.h"

#include <Engine/DataStructures/HashTable.h>
#include <Engine/DataStructures/DynamicArray.h>

class ResourceManager
{
private:	
	// Stores all loaded meshes
	DynamicArray<MeshResource*>			meshResources;
	DynamicArray<MeshColliderResource*> meshColliderResources;
	HashTable<String, ModelResource*>	modelResources;
	HashTable<String, TextureResource*> textureResources;

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
	const ModelResource		*LoadModel(const char *path);

	static ResourceManager &Instance();
};