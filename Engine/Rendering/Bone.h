#pragma once

#include "VertexWeight.h"
#include "ModelNode.h"

#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Math/Matrix.h>

class Bone
{
	friend class ResourceManager;

private:
	// Skeleton, to which this bone is attached
	Skeleton			*skeleton;

	// Bone tranformation in default pose
	Matrix4				localOffsetMatrix;
	// Inverse global (model) bone transformation in default pose
	Matrix4				inverseOffsetMatrix;

	// Node to get transformation from
	const ModelNode		*modelNode;
	
	// Current bone ID, bones are stored in skeleton class
	int					boneID;
	// Parent bone ID, bones are stored in skeleton class
	int					parentBoneID;
	// Child bones IDs, bones are stored in skeleton class
	DynamicArray<int>	childIDs;

public:
	// Default constructor
	Bone(Skeleton *skeleton, int boneID, const ModelNode *modelNode, const Matrix4 &localOffsetMatrix);
	// Allocate dynamic memory for class members
	void Init();

	// Copy
	// Note: doesn't copy child ids array
	void operator=(const Bone &source);

	// Get bone tranformation in default pose
	const Matrix4		&GetOffsetMatrix() const;
	// Get inverse global (model) bone transformation in default pose
	const Matrix4		&GetInverseGlobalOffsetMatrix() const;
	const ModelNode		&GetModelNode() const;

	int GetBoneID() const;
	int GetParentBoneID() const;
	const DynamicArray<int> GetChildBonesID() const;

	void SetParentBone(int id);
	void AddChildBone(int id);

	// Calculate inverse global (model) bone transformation in default pose
	void CalculateInverseMatrix(const Matrix4 &parentBoneOffset);
};
