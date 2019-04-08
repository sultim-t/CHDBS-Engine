#pragma once
#include "VertexWeight.h"
#include "ModelNode.h"
#include <Engine/Math/Matrix.h>

#define BONE_MAX_WEIGHTS 4

class Bone
{
private:
	// Matrix for tranformation from bone's local space to its world space
	Matrix4			offsetMatrix;

	// How many weights to use
	int				weightCount;
	// Array of weights
	VertexWeight	weights[BONE_MAX_WEIGHTS];
	// Name of this bone
	String			name;

	// Parent bone
	const Bone		*parent;
	// Node to get transformation from
	const ModelNode	*modelNode;

public:
	// Constructor
	inline Bone(const char *name, const Bone *parent, const ModelNode *modelNode, int weightCount, const Matrix4 &offsetMatrix);

	// Set weight, "index" is index of weight
	inline void					SetWeight(int index, const VertexWeight &weight);

	// Get weight, "index" is index of weight
	inline const VertexWeight	&GetWeight(int index) const;
	// Get bone's offset matrix, which tranforms from bone's local space to its world space
	inline const Matrix4		&GetOffsetMatrix() const;
	// How many weights are used
	inline int					GetWeightCount() const;

	// Get matrix with all parents' tranformations
	inline Matrix4	GetTranformationMatrix() const;
};

inline Bone::Bone(const char *name, const Bone *parent, const ModelNode *modelNode, int weightCount, const Matrix4 &offsetMatrix)
	: parent(parent), modelNode(modelNode), weightCount(weightCount), offsetMatrix(offsetMatrix)
{
	ASSERT(weightCount >= 0 && weightCount <= BONE_MAX_WEIGHTS);
	
	// bone is value type, so it will be allocated through malloc() in data structures
	this->name.Init(name);
}

inline void Bone::SetWeight(int index, const VertexWeight &weight)
{
	ASSERT(index >= 0 && index < weightCount);
	weights[index] = weight;
}

inline const VertexWeight & Bone::GetWeight(int index) const
{
	ASSERT(index >= 0 && index < weightCount);
	return weights[index];
}

inline int Bone::GetWeightCount() const
{
	return weightCount;
}

inline Matrix4 Bone::GetTranformationMatrix() const
{
	Matrix4 result = Matrix4(1.0f, true);
	const Bone* b = parent;

	while (b != nullptr)
	{
		// get parent bone tranformation
		const Matrix4 &parentTransform = b->modelNode->GetTransform();
		
		// convert to parent's space
		// engine uses transposed tranformation matrices
		result *= parentTransform;

		b = b->parent;
	}

	return result;
}

inline const Matrix4 &Bone::GetOffsetMatrix() const
{
	return offsetMatrix;
}