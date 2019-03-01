#pragma once

#include "MeshResource.h"
#include "TextureResource.h"
#include "ModelResource.h"

#include <Engine/Rendering/Texture.h>
#include <Engine/DataStructures/HashTable.h>
#include <Engine/DataStructures/DynamicArray.h>

class ResourceManager
{
private:	
	// Stores all loaded meshes
	DynamicArray<MeshResource*> meshResources;

	HashTable<String, ModelResource*> modelResources;

private:
	void ProcessModelNode(void *n, const void *s, ModelResource *outModel);
	MeshResource *ProcessMesh(void *m, const void *s);
	// std::vector<ITexture> LoadMaterialTextures(void * m, int t, TextureType myType, CModel &outModel);
	
public:
	~ResourceManager();
	
	void Init();

	UBYTE *LoadTexture(char const *filename, int *width, int *height, int *comp, int req_comp);
	void DeleteTexture(void *address);

	const ModelResource *LoadModel(const char *path);

	static ResourceManager &Instance();
};