#pragma once

enum TextureType
{
	TEXTURE_TYPE_DIFFUSE	= 0,
	TEXTURE_TYPE_SPECULAR	= 1,
	TEXTURE_TYPE_NORMAL		= 2,
	TEXTURE_TYPE_HEIGHT		= 3,
	TEXTURE_TYPE_EMISSION	= 4,
	TEXTURE_TYPE_DETAIL		= 5,
	TEXTURE_TYPE_CUBEMAP	= 6,
	TEXTURE_TYPE_SHADOWMAP	= 7
};

// all texture names must be same size
// last char is for index
#define TEXTURE_NAME_DIFFUSE	"t_diff0"
#define TEXTURE_NAME_SPECULAR	"t_spec0"
#define TEXTURE_NAME_NORMAL		"t_norm0"
#define TEXTURE_NAME_HEIGHT		"t_hght0"
#define TEXTURE_NAME_EMISSION	"t_emis0"
#define TEXTURE_NAME_DETAIL		"t_detl0"
#define TEXTURE_NAME_CUBEMAP	"t_cube0"
#define TEXTURE_NAME_SHADOWMAP	"t_shdw0"

// index of varying char in texture name
#define TEXTURE_NAME_LENGTH		7
#define TEXTURE_TYPES_COUNT		8

class ITexture
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
	virtual void Activate(INT i) const {};

	TextureType GetType() const;
	const char *GetPath() const;
	UINT GetID() const;
	INT GetWidth() const;
	INT GetHeight() const;
	UINT GetFormat() const;

	void SetType(TextureType t);
};

class Texture : public ITexture
{
public:
	Texture();
	~Texture();

	void Load(const char* path);
	void Activate(INT i) const override;
};
