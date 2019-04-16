#pragma once

#include <Engine/Rendering/Shader.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/DataStructures/StaticStack.h>

class Material
{
	friend class RenderingSystem;

private:
	MaterialID materialId;

protected:
	// current shader to use
	Shader shader;

	// must be <= 16
	StaticStack<const ITexture*> textures;

public:
	// Empty constructor
	Material();

	// Init material
	void Init();

	void Use();
	// Activate material's textures
	void ActivateTextures() const;
	// Add texture to material
	void AddTexture(const ITexture *t);

	// Bind shader
	void BindShader(const Shader &shader);

	// Getters
	Shader &GetShader();
	const Shader &GetShader() const;
};