#pragma once

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Math/Transform.h>
#include "Animation.h"

class ModelNode
{
	friend class ResourceManager;

private:
	// Pointer to parent node, null if root
	const ModelNode *parent;

	// Child nodes
	StaticArray<ModelNode*> childNodes;

	// Current node transformation
	Matrix4 transformMatrix;

	// Current meshes in node
	// Contains indices to meshes in ModelHierarchy
	StaticArray<int> meshes;

	String name;

public:
	// Constructor
	inline ModelNode(const char *name, const ModelNode *parent, const Matrix4 &transformMatrix, int childNodesCount, int meshesCount);
	// Destroys child nodes
	inline ~ModelNode();

	inline const String				&GetName() const;
	// Get parent node
	inline const ModelNode			&GetParent() const;
	// Get local tranformation matrix
	inline const Matrix4			&GetTransform() const;
	// Get global (model) tranformation matrix
	inline const Matrix4			GetGlobalTransform() const;
	// Get indices of meshes which are connected to this node
	inline const StaticArray<int>	&GetMeshes() const;
	
	// Get animated local tranformation matrix
	inline Matrix4 GetTransform(const Animation &animation, float time) const;
	// Get animated global (model) tranformation matrix
	inline Matrix4 GetGlobalTransform(const Animation &animation, float time) const;

	inline const StaticArray<ModelNode*> &GetChildNodes() const;
};

inline ModelNode::ModelNode(const char *name, const ModelNode *parent, const Matrix4 &transformMatrix, int childNodesCount, int meshesCount) :
	parent(parent), transformMatrix(transformMatrix)
{ 
	this->childNodes.Init(childNodesCount);
	this->meshes.Init(meshesCount);

	this->name.Init(name);
}

inline ModelNode::~ModelNode()
{
	for (UINT i = 0; i < childNodes.GetSize(); i++)
	{
		// delete recursively
		delete childNodes[i];
	}
}

inline const String & ModelNode::GetName() const
{
	return name;
}

inline const ModelNode &ModelNode::GetParent() const
{
	return *parent;
}

inline const Matrix4 &ModelNode::GetTransform() const
{
	return transformMatrix;
}

inline const Matrix4 ModelNode::GetGlobalTransform() const
{
	Matrix4 result = Matrix4(transformMatrix);

	const ModelNode *current = parent;

	while (current != nullptr)
	{
		result *= current->GetTransform();
		current = current->parent;
	}

	return result;
}

inline const StaticArray<int> &ModelNode::GetMeshes() const
{
	return meshes;
}

inline Matrix4 ModelNode::GetTransform(const Animation & animation, float time) const
{
	// base local tranformation
	Matrix4 result = Matrix4(transformMatrix);

	// try to find animation node with the same name
	const AnimationNode *animNode = animation.FindAnimationNode(name);

	// if found
	if (animNode != nullptr)
	{
		Vector3 position;
		Quaternion quat;
		Vector3 scale;

		// 1) animate position
		if (animNode->GetInterpolatedPosition(time, position))
		{
			result = Transform::TranslateMatrix(result, position);
		}

		// 2) animate scale
		/*if (animNode->GetInterpolatedScale(time, scale))
		{
			result = Transform::TranslateMatrix(result, scale);
		}*/

		// 3) animate rotation
		if (animNode->GetInterpolatedRotation(time, quat))
		{
			result = Transform::RotateMatrix(result, quat);
		}
	}

	return result;
}

inline Matrix4 ModelNode::GetGlobalTransform(const Animation & animation, float time) const
{
	Matrix4 result = Matrix4(transformMatrix);

	const ModelNode *current = parent;

	while (current != nullptr)
	{
		result *= current->GetTransform(animation, time);
		current = current->parent;
	}

	return result;
}

inline const StaticArray<ModelNode*> &ModelNode::GetChildNodes() const
{
	return childNodes;
}
