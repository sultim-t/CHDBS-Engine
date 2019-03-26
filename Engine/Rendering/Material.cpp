#include "Material.h"
#include <Engine/TEMP/tinyxml2/tinyxml2.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/Cubemap.h>

Material::Material(std::vector<ITexture> textures)
{
	this->textures = textures;
}

bool Material::Init(void * xmlElem)
{
	ASSERT(0);
	return true;
}

void Material::Use()
{
	shader.Use();
}

void Material::ActivateTextures() const
{
	UINT textureCounts[TEXTURE_TYPES_COUNT];
	for (int i = 0; i < TEXTURE_TYPES_COUNT; i++)
	{
		textureCounts[i] = 0;
	}

	for (UINT i = 0; i < textures.size(); i++)
	{
		const ITexture &t = textures[i];

		// bind to correct texture unit
		const char *tname = nullptr;
		TextureType ttype = t.GetType();
		int type = (int)ttype;
		int count = textureCounts[type];

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

		// last char is a texture's index
		name[TEXTURE_NAME_LENGTH - 1] = '0' + count;
		// null termianted string
		name[TEXTURE_NAME_LENGTH] = '\0';

		// set current texture
		shader.SetInt(name, type);

		switch (ttype)
		{
		case TextureType::Cubemap:
			// cubemaps are activated in special way
			((Cubemap&)t).ActivateCubemap(type);
			break;
		default:
			t.Activate(type);
			break;
		}

		textureCounts[type]++;
	}
}

void Material::AddTexture(const ITexture &t)
{
	textures.push_back(t);
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
