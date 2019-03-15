#pragma once

enum class TextureType
{
	Diffuse		= 0,
	Specular	= 1,
	Normal		= 2,
	Height		= 3,
	Emission	= 4,
	Detail		= 5,
	Cubemap		= 6,
	Shadowmap	= 7
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

	int width, height;
	int channelsNumber;
	UINT format;

	TextureType type;

public:
	void Activate(int i) const;

	TextureType GetType() const;
	UINT GetID() const;
	int GetWidth() const;
	int GetHeight() const;
	UINT GetFormat() const;

	void SetType(TextureType t);
};

class Texture : public ITexture
{
private:
	const TextureResource *textureResource;

public:
	Texture();
	~Texture();

	const TextureResource &GetTextureResource() const;

	void Init(const char* path);
};
