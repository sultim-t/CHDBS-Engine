#pragma once

#include "IComponent.h"
#include <Engine/Components/CMesh.h>
#include <Engine/ResourceManager/ModelResource.h>

class CModel : public IComponent
{
	friend class ResourceManager;

	CLASSDECLARATION(CModel)

public:
	UINT modelId;

	std::vector<Mesh> meshes;
	std::vector<ITexture> loadedTextures;
	const ModelResource *modelResource;

	String path;

public:
	bool IsCastingShadows;
	bool IsReceivingShadows;

public:
	CModel();

	void Init() override;
	void Update() override;
	void SetProperty(const String &key, const String &value) override;

	void BindPath(const char *path);
	//void Draw();
	//std::vector<Mesh> &CModel::GetMeshes();
	const std::vector<Mesh> &GetMeshes() const;
};