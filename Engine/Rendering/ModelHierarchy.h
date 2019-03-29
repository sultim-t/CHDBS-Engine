#pragma once

#include "ModelNode.h"
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/DataStructures/Array.h>
#include <Engine/Math/Matrix.h>

// Represents hierachy of meshes
// Note: doesn't contain textures and materials,
//       they should be loaded and setted manually
// Note: doesn't contain cameras and lights
class ModelHierarchy
{
	// Memory is deallocated there where it was allocated.
	// Resource manager just loads info to this class
	// but doesn't allocates current StaticArrays.
	friend class ResourceManager;

private:
	// Root node of model
	ModelNode *rootNode;

	// All meshes in this model
	StaticArray<MeshResource*>	meshes;
	// All animation in this model
	StaticArray<Animation*>		animations;

	// All temporary tranformations in this model
	// Must be recalculated before rendering
	mutable StaticArray<Matrix4> tranforms;

private:
	// Processes node's global transformations
	void GetTranformsFromNode(const ModelNode *node, const Matrix4 &parentGlobal) const;

public:
	// Allocates memory
	inline ModelHierarchy(ModelNode *rootNode, int meshesCount, int animationsCount);
	// Deallocates memory
	inline ~ModelHierarchy();

	// Returns meshes array
	// Note: meshes can't be deformed
	inline const StaticArray<MeshResource*> &GetMeshes() const;
	// Returns animations array
	// Note: animations can't be deformed
	inline const StaticArray<Animation*> &GetAnimations() const;

	// Calculate tranformation matrices foreach mesh
	//    "global" is global tranformation matrix
	// Note: memory is not allocated,
	//       returns reference to class member
	const StaticArray<Matrix4> &GetTranforms(const Matrix4 &global) const;

	inline bool IsAnimated() const;
};

inline ModelHierarchy::ModelHierarchy(ModelNode * rootNode, int meshesCount, int animationsCount) :
	rootNode(rootNode)
{
	// allocate
	meshes.Init(meshesCount); 
	animations.Init(animationsCount);
	tranforms.Init(meshesCount);
}

inline ModelHierarchy::~ModelHierarchy()
{
	// delete root node
	// all children nodes will be deleted in ModelNode class
	delete rootNode;

	// static arrays are deleted automatically 
}

inline const StaticArray<MeshResource*> &ModelHierarchy::GetMeshes() const
{
	return meshes;
}

inline const StaticArray<Animation*> &ModelHierarchy::GetAnimations() const
{
	return animations;
}

inline bool ModelHierarchy::IsAnimated() const
{
	return animations.GetSize() > 0;
}

inline void ModelHierarchy::GetTranformsFromNode(const ModelNode *node, const Matrix4 &parentGlobal) const
{
	// current node's transformation
	Matrix4 current = node->GetTransform() * parentGlobal;

	// get meshes' indices
	auto &nodeMeshes = node->GetMeshes();

	// bind foreach mesh in current node
	for (UINT i = 0; i < nodeMeshes.GetSize(); i++)
	{
		tranforms[nodeMeshes[i]] = parentGlobal;
	}

	// get all child nodes
	auto &children = node->GetChildNodes();

	// process them
	for (UINT i = 0; i < children.GetSize(); i++)
	{
		GetTranformsFromNode(children[i], current);
	}
}

inline const StaticArray<Matrix4> &ModelHierarchy::GetTranforms(const Matrix4 &global) const
{
	// recalulate tranforms wiht current global tranformation
	GetTranformsFromNode(rootNode, global);

	// return classs member
	return tranforms;
}
