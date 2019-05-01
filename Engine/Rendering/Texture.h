#pragma once
#include "ITexture.h"

// Represents texture prepared for rendering
class Texture : public ITexture
{
private:
	// Contains only a reference to texture resource
	const TextureResource *textureResource;

public:
	// Default constructor
	Texture();
	// Empty destructor, doesn't deletes texture resource
	~Texture();

	// Load texture from file
	void Load(const char* path);

	// Get current texture resource
	const TextureResource &GetTextureResource() const;

	// Activate this texture
	void Activate(int i) const override;
	// Deactivate this texture
	void Deactivate() const override;
};
