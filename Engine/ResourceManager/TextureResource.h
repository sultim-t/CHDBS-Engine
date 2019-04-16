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
	inline TextureResource(BYTE *data, const char *path, int width, int height, int channelsNumber);

	inline const String &GetPath() const;
	inline const BYTE	*GetData() const;
	inline const int	GetWidth() const;
	inline const int	GetHeight() const;
	inline const TextureColorFormat GetFormat() const;

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
