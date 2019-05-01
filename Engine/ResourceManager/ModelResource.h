#pragma once
#include <Engine/DataStructures/Array.h>
#include <Engine/Rendering/ModelHierarchy.h>
#include <Engine/Rendering/Materials/StandardMaterial.h>

// Contains initialized 
class ModelResource
{	
	// Memory is deallocated there where it was allocated.
	// Resource manager just loads info to this class
	// but doesn't allocates current StaticArrays.
	friend class ResourceManager;

public:
	// Model's hierarchy
	ModelHierarchy					*hierarchy;

	// All materials in this model
	mutable
	StaticArray<StandardMaterial*>	materials;

	String							path;

public:
	inline ModelResource(const char *path, ModelHierarchy *hierarchy, int materialCount);
	inline ~ModelResource();

	inline const ModelHierarchy			&GetHierarchy() const;
	inline const String					&GetPath() const;
	inline const StaticArray<StandardMaterial*> &GetMaterials() const;
	inline UINT							GetMeshesCount() const;
};

inline ModelResource::ModelResource(const char *path, ModelHierarchy *hierarchy, int materialCount)
	: path(path), hierarchy(hierarchy)
{
	this->materials.Init(materialCount);

	// init each material
	for (int i = 0; i < materialCount; i++)
	{
		// every model uses standard material
		materials[i] = new StandardMaterial();
		materials[i]->Init();
	}
}

inline ModelResource::~ModelResource()
{
	delete hierarchy;

	for (UINT i = 0; i < materials.GetSize(); i++)
	{
		delete materials[i];
	}
}

inline const ModelHierarchy &ModelResource::GetHierarchy() const
{
	return *hierarchy;
}

inline const String &ModelResource::GetPath() const
{
	return path;
}

inline const StaticArray<StandardMaterial*> &ModelResource::GetMaterials() const
{
	return materials;
}

inline UINT ModelResource::GetMeshesCount() const
{
	return hierarchy->GetMeshes().GetSize();
}