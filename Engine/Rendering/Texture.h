#pragma once

enum TextureType
{
	TEXTURE_DIFFUSE = 0,
	TEXTURE_SPECULAR = 1,
	TEXTURE_NORMAL = 2,
	TEXTURE_HEIGHT = 3,
	TEXTURE_EMISSION = 4,
	TEXTURE_DETAIL = 5,
	TEXTURE_CUBEMAP = 6
};

class Texture
{
	friend class RenderingSystem;

protected:
	TextureID textureId;
	UINT graphicsTextureId;

	INT width, height;
	INT channelsNumber;
	UINT format;
	
	TextureType type;
	const char* path;

public:
	Texture();
	~Texture();

	void Load(const char* path);
	virtual void Activate(INT i) const;

	UINT GetID() const;
	void SetType(TextureType t);
	TextureType GetType() const;
	const char *GetPath() const;
	INT GetWidth() const;
	INT GetHeight() const;
	UINT GetFormat() const;
};
