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
	StaticStack<ITexture*> textures;

private:
	static void GetTextureName(const ITexture &t, char *name);

protected:
	// Activate additional textures
	virtual void ActivateAdditional() const {};
	// Deactivate additional textures
	virtual void DeactivateAdditional() const {};

public:
	// Empty constructor
	Material();
	// Default destructor
	~Material();

	// Init material
	void Init();
	// Bind shader by its name
	void BindShader(const char *name);
	// Add texture to material
	void AddTexture(ITexture *t);
	
	// Get all uniform locations from shader
	virtual void InitUniformLocations() {};

	// Activate shader
	void Use();
	// Deactivate shader
	void Stop();

	// Activate material's textures
	void ActivateTextures() const;
	// Deactivate material's textures
	void DeactivateTextures() const;

	// Getters
	// Shader &GetShader();
	const Shader &GetShader() const;
};