#pragma once

#include "IComponent.h"
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Matrix.h>
#include <Engine/ResourceManager/ModelResource.h>

// Represents static model
class CModel : public IComponent
{
	CLASSDECLARATION(CModel)

public:
	UINT modelId;

	// Resource, loaded from file
	const ModelResource	*modelResource;

	// Arrays for rendering
	StaticArray<UINT> vaos, vbos, ibos;

	// Path to file
	String path;

public:
	// Does model cast shadows?
	bool IsCastingShadows;
	// Does model receive shadows?
	bool IsReceivingShadows;

private:
	void InitMesh(const MeshResource *resource, UINT &vao, UINT &vbo, UINT &ibo, bool useTan, bool dynamic);
	// Init as static model
	void InitStatic();
	// Init as dynamic (deformable) model
	void InitDynamic();

public:
	// Default constructor
	CModel();

	// Loads model from file
	void Init() override;
	void Update() override;
	void SetProperty(const String &key, const String &value) override;

	const StaticArray<MeshResource*> &GetMeshes() const;
	const StaticArray<Material*> &GetMaterials() const;
	const StaticArray<UINT> &GetVAO() const;

	// Calculate tranformation matrices foreach mesh
	// with current entity's tranform
	const StaticArray<Matrix4> &GetTranforms() const;

	// Calculate tranformation matrices foreach mesh
	// with current entity's tranform converted to global space
	const StaticArray<Matrix4> &GetTranforms(const Matrix4 &global) const;
};

inline const StaticArray<Matrix4> &CModel::GetTranforms() const
{
	return modelResource->GetHierarchy().GetTranforms(owner->GetTransform().GetTransformMatrix());
}

inline const StaticArray<Matrix4>& CModel::GetTranforms(const Matrix4 &global) const
{
	return modelResource->GetHierarchy().GetTranforms(owner->GetTransform().GetTransformMatrix() * global);
}
