#pragma once
#include <Engine/Rendering/TextureColorFormat.h>
#include <Engine/Memory/Memory.h>

class TextureResource
{
private:
	BYTE *data;
	
	int width, height;
	TextureColorFormat format;

	String path;

public:
	TextureResource(BYTE *data, const char *path, int width, int height, int channelsNumber);
	~TextureResource();

	const String &GetPath() const;
	const BYTE	*GetData() const;
	const int	GetWidth() const;
	const int	GetHeight() const;
	const TextureColorFormat GetFormat() const;

	inline void Delete();
};

inline TextureResource::TextureResource(BYTE *data, const char *path, int width, int height, int channelsNumber)
{
	this->data = data;
	this->path = path;
	this->width = width;
	this->height = height;
	this->format = channelsNumber == 1 ? TextureColorFormat::R : 
		channelsNumber == 3 ? TextureColorFormat::RGB : TextureColorFormat::RGBA;
}

inline TextureResource::~TextureResource()
{
	Delete();
}

inline const String &TextureResource::GetPath() const
{
	return path;
}

inline const BYTE *TextureResource::GetData() const
{
	return data;
}

inline const int TextureResource::GetWidth() const
{
	return width;
}

inline const int TextureResource::GetHeight() const
{
	return height;
}

inline const TextureColorFormat TextureResource::GetFormat() const
{
	return format;
}

inline void TextureResource::Delete()
{
	SystemAllocator::Free(data);
}
