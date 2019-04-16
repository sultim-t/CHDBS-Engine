#include "ITexture.h"

UINT ITexture::GetID() const
{
	return graphicsTextureId;
}

void ITexture::SetType(TextureType t)
{
	type = t;
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

