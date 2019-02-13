#pragma once

#include "IComponent.h"
#include <Engine/Components/CMesh.h>

class CModel : public IComponent
{
	CLASSDECLARATION(CModel)

public:
	std::vector<Mesh> meshes;
	std::vector<Texture> loadedTextures;

	std::string directory;
	unsigned modelId;

public:
	//~CModel() override;
	// Load through assimp
	void Load(const char *path);

	void Draw();

	const std::vector<Mesh> &GetMeshes() const;

public:
	void Init() override;
	void Update() override;
};