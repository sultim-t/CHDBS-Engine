#pragma once

#include <Engine\Rendering\Shader.h>
#include <Engine\Rendering\Texture.h>
#include <vector>

class Material
{
	friend class RenderingSystem;

private:
	MaterialID materialId;

protected:
	Shader shader;
	// must be <= 16
	std::vector<ITexture> textures;

public:
	Material(std::vector<ITexture> textures = std::vector<ITexture>());

	// Init material from XML element
	bool Init(void* xmlElem);

	void Use();
	// Activate material's textures
	void ActivateTextures() const;
	// Add texture to material
	void AddTexture(const ITexture &t);

	// Bind shader
	void BindShader(const Shader &shader);

	// Getters
	Shader &GetShader();
	const Shader &GetShader() const;
};