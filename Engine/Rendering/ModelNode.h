#pragma once

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Transform.h>
#include <Engine/Math/Matrix.h>

class ModelNode
{
private:
	// Pointer to parent node, null if root
	const ModelNode *parent;

	// Child nodes
	StaticArray<ModelNode*> childNodes;

	// Current node transformation
	Transform transform;
	// Current node transformation
	Matrix4 transformMatrix;

	// Current meshes in node
	// Contains indices to meshes in ModelHierarchy
	StaticArray<int> meshes;

public:
	// Constructor
	inline ModelNode(
		const ModelNode *parent,
		const StaticArray<ModelNode*> &childNodes,
		const Matrix4 &transformMatrix,
		const StaticArray<int> &meshes);

	inline const ModelNode			&GetParent() const;
	inline const Matrix4			&GetTransform() const;
	inline const StaticArray<int>	&GetMeshes() const;
	inline const StaticArray<ModelNode*> &GetChildNodes() const;
};

inline ModelNode::ModelNode(const ModelNode *parent, const StaticArray<ModelNode*> &childNodes, const Matrix4 &transformMatrix, const StaticArray<int> &meshes)
{
	this->parent = parent;
	this->childNodes = childNodes;
	this->transformMatrix = transformMatrix;
	this->meshes = meshes;
}

inline const ModelNode &ModelNode::GetParent() const
{
	return *parent;
}

inline const Matrix4 &ModelNode::GetTransform() const
{
	return transformMatrix;
}

inline const StaticArray<int> &ModelNode::GetMeshes() const
{
	return meshes;
}

inline const StaticArray<ModelNode*> &ModelNode::GetChildNodes() const
{
	return childNodes;
}
