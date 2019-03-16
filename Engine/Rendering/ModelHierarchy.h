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
public:
	// Root node of model
	ModelNode *rootNode;

	// All meshes in this model
	StaticArray<MeshResource*>	meshes;
	// All animation in this model
	StaticArray<Animation*>		animations;

public:
	inline ModelHierarchy(
		ModelNode *rootNode, 
		const StaticArray<MeshResource*> &meshes,
		const StaticArray<Animation*> &animations);
	inline ~ModelHierarchy();

	inline bool IsAnimated() const;
};

inline ModelHierarchy::ModelHierarchy(ModelNode *rootNode, const StaticArray<MeshResource*>& meshes, const StaticArray<Animation*>& animations)
{
	this->rootNode = rootNode;
	this->meshes = meshes;
	this->animations = animations;
}

inline ModelHierarchy::~ModelHierarchy()
{
	// delete root node
	// all children nodes will be deleted in ModelNode class
	delete rootNode;
}

inline bool ModelHierarchy::IsAnimated() const
{
	return animations.GetSize() > 0;
}