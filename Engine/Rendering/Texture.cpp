#include "Texture.h"
#include "OpenGL.h"
#include <Engine/ResourceManager/ResourceManager.h>

Texture::Texture()
{
	type = TEXTURE_DIFFUSE;
}

Texture::~Texture()
{
}

void Texture::Load(const char * path)
{
	glGenTextures(1, &graphicsTextureId);

	UBYTE *data = ResourceManager::LoadTexture(path, &width, &height, &channelsNumber, 0);

	if (data)
	{
		if (channelsNumber == 1)
		{
			format = GL_RED;
		}
		else if (channelsNumber == 3)
		{
			format = GL_RGB;
		}
		else if (channelsNumber == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, graphicsTextureId);
		// copy data
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		Logger::Print("Can't load texture: %s"); //, path);
	}

	// data is copied, clear
	ResourceManager::DeleteTexture(data);

	this->path = path;
}

void Texture::Activate(INT i) const
{
	ASSERT(i >= 0 && i < 32);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, graphicsTextureId);
}