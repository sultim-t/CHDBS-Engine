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
	std::vector<TextureID> textures;

public:
	// Init material from XML element
	bool Init(void* xmlElem);

	// Activate material
	void Activate();
};