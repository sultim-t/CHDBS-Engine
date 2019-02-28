#pragma once
#include <Engine/Rendering/TextureColorFormat.h>

class TextureResource
{
private:
	UINT *data;
	
	int width, height;
	TextureColorFormat format;

	String path;

private:
	TextureResource();

public:
	inline TextureResource(UINT *data, const char *path, int width, int height, int channelsNumber);
	inline ~TextureResource();

	inline const String &GetPath() const;
	inline const UINT *GetData() const;
	inline const int GetWidth() const;
	inline const int GetHeight() const;
	inline const TextureColorFormat GetFormat() const;
};

inline TextureResource::TextureResource(UINT *data, const char *path, int width, int height, int channelsNumber)
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
	SYSALLOCATOR.Free(data);
}

inline const String & TextureResource::GetPath() const
{
	return path;
}

inline const UINT * TextureResource::GetData() const
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
