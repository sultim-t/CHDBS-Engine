#include "Material.h"
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/Cubemap.h>
#include <Engine/Systems/RenderingSystem.h>

Material::Material()
{ }

Material::~Material()
{ 
	for (int i = 0; i < textures.GetTop(); i++)
	{
		// delete texture holders,
		// but not textures (TextureResource) themselves
		delete textures[i];
	}
}

void Material::Init()
{
	// max texture count
	textures.Init(16);
}

void Material::Use()
{
	// activate shader
	shader->Use();
}

void Material::Stop()
{	
	shader->Stop();
}

void Material::ActivateTextures() const
{
	// for each texture
	UINT textureCount = textures.GetTop();
	for (UINT i = 0; i < textureCount; i++)
	{
		const ITexture &t = *textures[i];
		int type = (int)t.GetType();

		char name[TEXTURE_NAME_LENGTH + 1];
		GetTextureName(t, name);

		// bind to correct texture unit
		shader->SetInt(name, type);

		// activate
		t.Activate(type);
	}

	ActivateAdditional();
}

void Material::DeactivateTextures() const
{
	// deactivate each texture
	UINT textureCount = textures.GetTop();
	for (UINT i = 0; i < textureCount; i++)
	{
		textures[i]->Deactivate();
	}

	DeactivateAdditional();
}

void Material::AddTexture(ITexture *t)
{
	// add texture
	textures.Push(t);
}

void Material::BindShader(const char *name)
{
	shader = RenderingSystem::Instance().GetShader(name);
	ASSERT(shader != nullptr);
}

const Shader &Material::GetShader() const
{
	return *shader;
}

void Material::GetTextureName(const ITexture &t, char *name)
{
	const char *tname = nullptr;

	switch (t.GetType())
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

	// copy except last char
	for (int i = 0; i < TEXTURE_NAME_LENGTH - 1; i++)
	{
		name[i] = tname[i];
	}

	// last char is a texture's count number
	name[TEXTURE_NAME_LENGTH - 1] = '0';
	// null termianted string
	name[TEXTURE_NAME_LENGTH] = '\0';
}