#pragma once
#include <Engine/DataStructures/Array.h>
#include <Engine/Rendering/ModelHierarchy.h>

// Contains initialized 
class ModelResource
{
private:
	// Model's hierarchy
	ModelHierarchy *hierarchy;

	// All materials in this model
	// Must be bijection with meshes 
	// TEMP mutable
	mutable
	StaticArray<Material*> materials;

	String path;

public:
	inline ModelResource(const char *path, ModelHierarchy *hierarchy);
	inline ~ModelResource();

	template <UINT Size>
	inline void SetMaterials(const Array<Material*, Size> &materials);
	// Set all materials
	inline void SetMaterials(Material *material) const
	{
		for (UINT i = 0; i < hierarchy->GetMeshes().GetSize(); i++)
		{
			this->materials[i] = material;
		}
	}

	inline const ModelHierarchy &GetHierarchy() const;
	inline const String &GetPath() const;
	inline const StaticArray<Material*> &GetMaterials() const;
	inline UINT GetMeshesCount() const;
};

inline ModelResource::ModelResource(const char *path, ModelHierarchy *hierarchy)
{
	this->path = path;
	this->hierarchy = hierarchy;

	UINT meshesCount = hierarchy->GetMeshes().GetSize();

	this->materials.Init(meshesCount);
}

inline ModelResource::~ModelResource()
{
	delete hierarchy;
}

inline const ModelHierarchy &ModelResource::GetHierarchy() const
{
	return *hierarchy;
}

inline const String &ModelResource::GetPath() const
{
	return path;
}

inline const StaticArray<Material*> &ModelResource::GetMaterials() const
{
	return materials;
}

inline UINT ModelResource::GetMeshesCount() const
{
	return hierarchy->GetMeshes().GetSize();
}

template<UINT Size>
inline void ModelResource::SetMaterials(const Array<Material*, Size> &materials)
{
	for (UINT i = 0; i < hierarchy->GetMeshes().GetSize(); i++)
	{
		this->materials[i] = materials[i];
	}
}