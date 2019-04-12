#pragma once

#include "VertexWeight.h"
#include "ModelNode.h"

#include <Engine/Math/Matrix.h>

class Bone
{
private:
	// Bone tranformation in default pose
	Matrix4			offsetMatrix;

	// Node to get transformation from
	const ModelNode	*modelNode;
	
	// Current bone ID, bones are stored in skeleton class
	int				boneID;
	// Parent bone ID
	int				parentBoneID;

public:
	// Default constructor
	inline Bone(int boneID, int parentBoneID, const ModelNode *modelNode, const Matrix4 &offsetMatrix);

	// Get bone tranformation in default pose
	inline const Matrix4		&GetOffsetMatrix() const;
	inline const ModelNode		&GetModelNode() const;

	inline int GetBoneID() const;
	inline int GetParentBoneID() const;
};

inline Bone::Bone(int boneID, int parentBoneID, const ModelNode * modelNode, const Matrix4 & offsetMatrix)
	: boneID(boneID), parentBoneID(parentBoneID), modelNode(modelNode), offsetMatrix(offsetMatrix) { }

inline const ModelNode &Bone::GetModelNode() const
{
	return *modelNode;
}

inline int Bone::GetBoneID() const
{
	return boneID;
}

inline int Bone::GetParentBoneID() const
{
	return parentBoneID;
}

inline const Matrix4 &Bone::GetOffsetMatrix() const
{
	return offsetMatrix;
}