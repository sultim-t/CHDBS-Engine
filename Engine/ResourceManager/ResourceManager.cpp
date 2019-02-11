#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <ImageLoading\stb_image.h>

UBYTE * ResourceManager::LoadTexture(char const * filename, INT * width, INT * height, INT * comp, INT req_comp)
{
	return stbi_load(filename, width, height, comp, req_comp);
}

void ResourceManager::DeleteTexture(void * address)
{
	stbi_image_free(address);
}
