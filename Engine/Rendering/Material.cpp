#include "Material.h"
#include <Engine/TEMP/tinyxml2/tinyxml2.h>
#include <Engine/Rendering/OpenGL.h>

Material::Material(std::vector<Texture> textures)
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
	for (UINT i = 0; i < textures.size(); i++)
	{
		const Texture &t = textures[i];

		// activate texture
		t.Activate(i);

		// bind to correct texture unit
		char tname[] = "texture_diffuse_";
		tname[15] = '0' + t.GetType();
		shader.SetInt(tname, i);
	}
}

void Material::AddTexture(Texture t)
{
	textures.push_back(t);
}

void Material::BindShader(Shader &shader)
{
	this->shader = shader;
}
