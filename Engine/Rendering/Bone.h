#pragma once
#include "VertexWeight.h"
#include <Engine/Math/Matrix.h>

#define BONE_MAX_WEIGHTS 4

class Bone
{
private:
	// Matrix for tranformation from bone to mesh space
	Matrix4			boneMesh;

	// How many weights to use
	int				weightCount;
	// Array of weights
	VertexWeight	weights[BONE_MAX_WEIGHTS];

public:
	// Constructor
	inline Bone(int weightCount, const Matrix4 &boneMesh);

	// Set weight, "index" is index of weight
	inline void					SetWeight(int index, const VertexWeight &weight);

	// Get weight, "index" is index of weight
	inline const VertexWeight	&GetWeight(int index) const;
	inline int					GetWeightCount() const;
	inline const Matrix4		&GetMatrix() const;
};

inline Bone::Bone(int weightCount, const Matrix4 &boneMesh)
{
	ASSERT(weightCount > 0 && weightCount <= BONE_MAX_WEIGHTS);

	this->weightCount = weightCount;
	this->boneMesh = boneMesh;
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

inline const Matrix4 &Bone::GetMatrix() const
{
	return boneMesh;
}
