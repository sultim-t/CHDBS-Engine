#include "Texture.h"
#include "OpenGL.h"
#include <Engine/ResourceManager/ResourceManager.h>

Texture::Texture()
{
	type = TextureType::Diffuse;
	textureResource = nullptr;
}

Texture::~Texture()
{ }

const TextureResource & Texture::GetTextureResource() const
{
	return *textureResource;
}

void Texture::Init(const char * path)
{
	textureResource = ResourceManager::Instance().LoadTexture(path);

	if (textureResource == nullptr)
	{
		return;
	}

	width = textureResource->GetWidth();
	height = textureResource->GetHeight();
	//channelsNumber = textureResource->GetFormat();

	if (textureResource->GetFormat() == TextureColorFormat::R)
	{
		format = GL_RED;
	}
	else if (textureResource->GetFormat() == TextureColorFormat::RGB)
	{
		format = GL_RGB;
	}
	else if (textureResource->GetFormat() == TextureColorFormat::RGBA)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &graphicsTextureId);
	glBindTexture(GL_TEXTURE_2D, graphicsTextureId);
	
	// copy data
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureResource->GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

UINT ITexture::GetID() const
{
	return graphicsTextureId;
}

void ITexture::SetType(TextureType t)
{
	type = t;
}

void ITexture::Activate(int i) const
{
	ASSERT(i >= 0 && i < 32);

	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, graphicsTextureId);
}

TextureType ITexture::GetType() const
{
	return type;
}

int ITexture::GetWidth() const
{
	return width;
}

int ITexture::GetHeight() const
{
	return height;
}

UINT ITexture::GetFormat() const
{
	return format;
}
