#include "Cubemap.h"
#include "OpenGL.h"

#include <Engine/DataStructures/StaticArray.h>

#include <ImageLoading/stb_image.h>

Cubemap::Cubemap()
{
	type = TEXTURE_TYPE_CUBEMAP;
}

void Cubemap::LoadCubemap(const StaticArray<const char*, 6> &paths)
{
	glGenTextures(1, &graphicsTextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, graphicsTextureId);

	for (unsigned int i = 0; i < paths.GetSize(); i++)
	{
		unsigned char *data = stbi_load(paths[i], &width, &height, &channelsNumber, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			Logger::Print("Can't load cubemap");
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Activate(INT i) const
{
	ASSERT(i >= 0 && i < 32);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_CUBE_MAP, graphicsTextureId);
}
