#pragma once

#include <Engine/Rendering/Shader.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/DataStructures/StaticStack.h>

class Material
{
	friend class RenderingSystem;

private:
	UINT materialId;

protected:
	// current shader to use
	const Shader *shader;

	// must be <= 16
	StaticStack<const ITexture*> textures;

public:
	// Empty constructor
	Material();

	// Init material
	void Init();
	// Bind shader by its name
	void BindShader(const char *name);
	// Add texture to material
	void AddTexture(const ITexture *t);

	// Activate shader
	void Use();
	// Deactivate shader
	void Stop();

	// Activate material's textures
	void ActivateTextures() const;
	// Deactivate material's textures
	void DeactivateTextures() const;

	// Getters
	Shader &GetShader();
	const Shader &GetShader() const;
};