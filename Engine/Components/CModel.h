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

private:
	void ProcessNode(void *node, const void *scene);
	Mesh ProcessMesh(void *mesh, const void *scene);
	std::vector<Texture> LoadMaterialTextures(void *mat, int type, TextureType myType);

public:
	//~CModel() override;

	bool operator<(const CModel &model) const;

	// Load through assimp
	void Load(const char *path);

	void Draw();

	const std::vector<Mesh> &GetMeshes() const;

public:
	void Init() override;
	void Update() override;
};