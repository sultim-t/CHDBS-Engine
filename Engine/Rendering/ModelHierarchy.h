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
private:
	// Root node of model
	ModelNode *rootNode;

	// All meshes in this model
	StaticArray<MeshResource*>	meshes;
	// All animation in this model
	StaticArray<Animation*>		animations;

public:
	inline bool IsAnimated() const;
};

inline bool ModelHierarchy::IsAnimated() const
{
	return animations.GetSize() > 0;
}