#pragma once

#include "ModelNode.h"
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Transform.h>
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

	inline bool IsAnimated() const;
};

inline ModelHierarchy::ModelHierarchy(ModelNode * rootNode, int meshesCount, int animationsCount) :
	rootNode(rootNode)
{
	// allocate
	meshes.Init(meshesCount); 
	animations.Init(animationsCount);
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