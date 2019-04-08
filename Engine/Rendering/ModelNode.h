#pragma once

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Matrix.h>

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
	inline const ModelNode			&GetParent() const;
	inline const Matrix4			&GetTransform() const;
	inline const StaticArray<int>	&GetMeshes() const;
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

inline const StaticArray<int> &ModelNode::GetMeshes() const
{
	return meshes;
}

inline const StaticArray<ModelNode*> &ModelNode::GetChildNodes() const
{
	return childNodes;
}
