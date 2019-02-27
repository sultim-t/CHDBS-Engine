#pragma once

#include "MeshResource.h"
#include <Engine/Rendering/Texture.h>
#include <Engine/DataStructures/HashTable.h>
#include <Engine/DataStructures/DynamicArray.h>

class ResourceManager
{
private:	
	// Stores all loaded meshes
	DynamicArray<MeshResource*> meshResources;

private:
	void ProcessModelNode(void * n, const void * s, CModel &outModel);
	Mesh ProcessMesh(void * m, const void * s, CModel &outModel);
	// std::vector<ITexture> LoadMaterialTextures(void * m, int t, TextureType myType, CModel &outModel);
	
public:
	~ResourceManager();
	
	void Init();

	UBYTE *LoadTexture(char const *filename, int *width, int *height, int *comp, int req_comp);
	void DeleteTexture(void *address);

	void LoadModel(const char *path, CModel &outModel);

	static ResourceManager &Instance();
};