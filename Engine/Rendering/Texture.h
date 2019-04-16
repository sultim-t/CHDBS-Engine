#pragma once
#include "ITexture.h"

class Texture : public ITexture
{
private:
	const TextureResource *textureResource;

public:
	// Default constructor
	Texture();
	// Empty destructor
	~Texture();

	// Init texture from file
	void Init(const char* path);

	// Get current texture resource
	const TextureResource &GetTextureResource() const;
	// Activate this texture
	void Activate(int i) const override;
};
