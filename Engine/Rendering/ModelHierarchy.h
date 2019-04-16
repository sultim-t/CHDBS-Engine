#pragma once

#include "ModelNode.h"
#include "Animation.h"
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/DataStructures/Array.h>
#include <Engine/Math/Matrix.h>
#include <Engine/ResourceManager/MeshResource.h>

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
	// Find node with given name
	inline const ModelNode *FindNode(const char *name, const ModelNode *current) const;

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
	inline const StaticArray<Matrix4> &GetTranforms(const Matrix4 &global) const;

	// Does at least one animation exist?
	inline bool IsAnimated() const;

	// Find node with given name
	inline const ModelNode *FindNode(const char *name) const;

	// Find node with given name
	inline const Animation *FindAnimation(const char *name) const;
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
	// all child nodes will be deleted in ModelNode class

	if (rootNode != nullptr)
	{
		delete rootNode;
	}

	for (UINT i = 0; i < animations.GetSize(); i++)
	{
		delete animations[i];
	}	
	
	for (UINT i = 0; i < meshes.GetSize(); i++)
	{
		delete meshes[i];
	}
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
	// engine uses tranposed transformation matrices (last row has position)
	// so to tranform from local to global:
	Matrix4 current = node->GetTransform() * parentGlobal;

	// get meshes' indices
	auto &nodeMeshes = node->GetMeshes();

	// bind foreach mesh in current node
	for (UINT i = 0; i < nodeMeshes.GetSize(); i++)
	{
		tranforms[nodeMeshes[i]] = current;
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

inline const ModelNode *ModelHierarchy::FindNode(const char *name) const
{
	return FindNode(name, rootNode);
}

inline const Animation *ModelHierarchy::FindAnimation(const char *name) const
{
	UINT count = animations.GetSize();
	for (UINT i = 0; i < count; i++)
	{
		if (animations[i]->GetName() == name)
		{
			return animations[i];
		}
	}

	return nullptr;
}

inline const ModelNode *ModelHierarchy::FindNode(const char *name, const ModelNode *current) const
{
	// if it's curren node
	if (current->GetName() == name)
	{
		return current;
	}

	// for each child
	const StaticArray<ModelNode*> &children = current->GetChildNodes();
	UINT count = children.GetSize();
	for (UINT i = 0; i < count; i++)
	{
		// try to find in children
		const ModelNode *found = FindNode(name, children[i]);

		if (found != nullptr)
		{
			// if found
			return found;
		}
	}

	return nullptr;
}