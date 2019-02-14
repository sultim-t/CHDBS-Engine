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

void Material::Activate() const
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
		const char *tname;
		int type = t.GetType();
		int count = textureCounts[type];

		switch (type)
		{
		case TEXTURE_TYPE_DIFFUSE:
			tname = TEXTURE_NAME_DIFFUSE;
			break;
		case TEXTURE_TYPE_SPECULAR:
			tname = TEXTURE_NAME_SPECULAR;
			break;
		case TEXTURE_TYPE_NORMAL:
			tname = TEXTURE_NAME_NORMAL;
			break;
		case TEXTURE_TYPE_HEIGHT:
			tname = TEXTURE_NAME_HEIGHT;
			break;
		case TEXTURE_TYPE_EMISSION:
			tname = TEXTURE_NAME_EMISSION;
			break;
		case TEXTURE_TYPE_DETAIL:
			tname = TEXTURE_NAME_DETAIL;
			break;
		case TEXTURE_TYPE_CUBEMAP:
			tname = TEXTURE_NAME_CUBEMAP;
			break;
		case TEXTURE_TYPE_SHADOWMAP:
			tname = TEXTURE_NAME_SHADOWMAP;
			break;
		}

		char name[TEXTURE_NAME_LENGTH];

		// copy except last char
		for (int i = 0; i < TEXTURE_NAME_LENGTH - 1; i++)
		{
			name[i] = tname[i];
		}

		// last char is a texture's index
		name[TEXTURE_NAME_LENGTH - 1] = '0' + count;

		shader.SetInt(name, type);
		// activate texture
		// doesnt work: wrong virtual overriding
		// t.Activate(type);

		if (type != TEXTURE_TYPE_CUBEMAP)
		{
			glActiveTexture(GL_TEXTURE0 + type);
			glBindTexture(GL_TEXTURE_2D, t.GetID());
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + type);
			glBindTexture(GL_TEXTURE_CUBE_MAP, t.GetID());
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

void Material::BindModelMatrix(const Matrix4 & model)
{
	shader.SetMat4("model", model);
}

Shader & Material::GetShader()
{
	return shader;
}

const Shader & Material::GetShader() const
{
	return shader;
}
