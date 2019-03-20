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
	StaticArray<Material*> materials;

	// Arrays for rendering
	StaticArray<UINT> vaos, vbos, ibos;

	String path;

private:
	void InitMesh(const MeshResource *resource, UINT &vao, UINT &vbo, UINT &ibo, bool useTan, bool dynamic);

public:
	inline ModelResource(const char *path, ModelHierarchy *hierarchy);

	// Init as static model
	void InitStatic();
	// Init as dynamic (deformable) model
	void InitDynamic();

	template <UINT Size>
	inline void SetMaterials(const Array<Material*, Size> &materials);

	inline const ModelHierarchy &GetHierarchy() const;
	inline const String &GetPath() const;
	inline const StaticArray<Material*> &GetMaterials() const;
	inline const StaticArray<UINT> &GetVAO() const;
};

inline ModelResource::ModelResource(const char *path, ModelHierarchy *hierarchy)
{
	this->path = path;
	this->hierarchy = hierarchy;

	UINT meshesCount = hierarchy->GetMeshes().GetSize();

	this->materials.Init(meshesCount);
	this->vaos.Init(meshesCount);
	this->vbos.Init(meshesCount);
	this->ibos.Init(meshesCount);
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

inline const StaticArray<UINT> &ModelResource::GetVAO() const
{
	return vaos;
}

template<UINT Size>
inline void ModelResource::SetMaterials(const Array<Material*, Size> &materials)
{
	for (UINT i = 0; i < hierarchy->GetMeshes().GetSize(); i++)
	{
		this->materials[i] = materials[i];
	}
}