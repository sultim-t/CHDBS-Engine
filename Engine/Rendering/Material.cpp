#include "Material.h"
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/Cubemap.h>

Material::Material()
{ }

void Material::Init()
{
	// max texture count
	textures.Init(16);
}

void Material::Use()
{
	// activate shader
	shader.Use();
}

void Material::Stop()
{	
	shader.Stop();
}

void Material::ActivateTextures() const
{
	// for each texture
	UINT textureCount = textures.GetTop();
	for (UINT i = 0; i < textureCount; i++)
	{
		const ITexture &t = *textures[i];

		// bind to correct texture unit
		const char *tname = nullptr;
		TextureType ttype = t.GetType();
		int type = (int)ttype;

		switch (ttype)
		{
		case TextureType::Diffuse:
			tname = TEXTURE_NAME_DIFFUSE;
			break;
		case TextureType::Specular:
			tname = TEXTURE_NAME_SPECULAR;
			break;
		case TextureType::Normal:
			tname = TEXTURE_NAME_NORMAL;
			break;
		case TextureType::Height:
			tname = TEXTURE_NAME_HEIGHT;
			break;
		case TextureType::Emission:
			tname = TEXTURE_NAME_EMISSION;
			break;
		case TextureType::Detail:
			tname = TEXTURE_NAME_DETAIL;
			break;
		case TextureType::Cubemap:
			tname = TEXTURE_NAME_CUBEMAP;
			break;
		case TextureType::Shadowmap:
			tname = TEXTURE_NAME_SHADOWMAP;
			break;
		}

		char name[TEXTURE_NAME_LENGTH + 1];

		// copy except last char
		for (int i = 0; i < TEXTURE_NAME_LENGTH - 1; i++)
		{
			name[i] = tname[i];
		}

		// last char is a texture's count number
		name[TEXTURE_NAME_LENGTH - 1] = '0';
		// null termianted string
		name[TEXTURE_NAME_LENGTH] = '\0';

		// set current texture
		shader.SetInt(name, type);

		// activate
		t.Activate(type);
	}
}

void Material::DeactivateTextures() const
{
	// deactivate each texture
	UINT textureCount = textures.GetTop();
	for (UINT i = 0; i < textureCount; i++)
	{
		textures[i]->Deactivate();
	}
}

void Material::AddTexture(const ITexture *t)
{
	// add texture
	textures.Push(t);
}

void Material::BindShader(const Shader &shader)
{
	this->shader = shader;
}

Shader &Material::GetShader()
{
	return shader;
}

const Shader &Material::GetShader() const
{
	return shader;
}
