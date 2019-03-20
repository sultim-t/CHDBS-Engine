#pragma once

#include "IComponent.h"
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Components/Mesh.h>
#include <Engine/ResourceManager/ModelResource.h>

// Represents static model
class CModel : public IComponent
{
	CLASSDECLARATION(CModel)

public:
	UINT modelId;

	// Resource, loaded from file
	const ModelResource	*modelResource;
	// Path to file
	String path;

public:
	// Does model cast shadows?
	bool IsCastingShadows;
	// Does model receive shadows?
	bool IsReceivingShadows;

public:
	// Default constructor
	CModel();

	void Init() override;
	void Update() override;
	void SetProperty(const String &key, const String &value) override;

	const StaticArray<MeshResource*> &GetMeshes() const;
	const StaticArray<Material*> &GetMaterials() const;
	const StaticArray<UINT> &GetVAO() const;
};