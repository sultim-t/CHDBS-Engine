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

	// Array of weights
	StaticArray<VertexWeight>	weights;
	// Name of this bone
	String			name;

	// Parent bone
	const Bone		*parent;
	// Node to get transformation from
	const ModelNode	*modelNode;

public:
	// Default constructor
	inline Bone(const Bone *parent, const ModelNode *modelNode, const Matrix4 &offsetMatrix);

	// Initialize dynamic structures
	inline void Init(const char *name, UINT weightCount);

	// Set weight, "index" is index of weight
	inline void					SetWeight(UINT index, const VertexWeight &weight);

	// Get name of this bone
	inline const String			&GetName() const;
	// Get weight, "index" is index of weight
	inline const VertexWeight	&GetWeight(UINT index) const;
	// Get bone's offset matrix, which tranforms from bone's local space to its world space
	inline const Matrix4		&GetOffsetMatrix() const;
	// How many weights are used
	inline UINT					GetWeightCount() const;
	inline const ModelNode		&GetModelNode() const;

	// Get matrix with all parents' tranformations
	inline Matrix4	GetTranformationMatrix() const;
};

inline Bone::Bone(const Bone *parent, const ModelNode *modelNode, const Matrix4 &offsetMatrix)
{
	this->parent = parent;
	this->modelNode = modelNode;
	this->offsetMatrix = offsetMatrix;
}

inline void Bone::Init(const char *name, UINT weightCount)
{
	// bone is value type, so it will be allocated through malloc() in data structures
	this->name.Init(name);
	this->weights.Init(weightCount);
}

inline void Bone::SetWeight(UINT index, const VertexWeight &weight)
{
	ASSERT(index >= 0 && index < weights.GetSize());
	weights[index] = weight;
}

inline const String &Bone::GetName() const
{
	return name;
}

inline const VertexWeight & Bone::GetWeight(UINT index) const
{
	ASSERT(index >= 0 && index < weights.GetSize());
	return weights[index];
}

inline UINT Bone::GetWeightCount() const
{
	return weights.GetSize();
}

inline const ModelNode & Bone::GetModelNode() const
{
	return *modelNode;
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