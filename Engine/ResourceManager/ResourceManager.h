#pragma once

#include <Engine/Rendering/Texture.h>
#include <vector>

class ResourceManager
{
private:
	static void ProcessModelNode(void * n, const void * s, CModel &outModel);
	static Mesh ProcessMesh(void * m, const void * s, CModel &outModel);
	static std::vector<Texture> LoadMaterialTextures(void * m, int t, TextureType myType, CModel &outModel);
	
public:
	static UBYTE *LoadTexture(char const *filename, INT *width, INT *height, INT *comp, INT req_comp);
	static void DeleteTexture(void *address);

	static void LoadModel(const char *path, CModel &outModel);
	static void LoadText();
	// void LoadXML();
};