#pragma once
#include "Bone.h"

class Skeleton
{
	friend class ResourceManager;

private:
	// Referenc to mesh which is affected by this skeleton
	const MeshResource		&mesh;

	// All bones in this skeleton
	// Order is not important
	StaticArray<Bone>		bones;

	// Bone weights for each vertex
	// Each element stores array of weights for each vertex
	StaticArray<VertexWeight>		vertexWeights;

	// Temp bone transformations
	// Is used to calculate animation
	mutable StaticArray<Matrix4>	bonesMatrices;

private:
	void UpdateBoneMatrices(const Matrix4 &global) const;
	Matrix4 GetBoneTranform(const Bone &bone) const;

public:
	// Allocates memory for bone and vertex arrays
	Skeleton(const MeshResource &meshToUse, int bonesCount, int verticesCount);

	// Update vertices according to the bones with global tranformation
	void UpdateVertices(const Matrix4 &global, StaticArray<Vertex5> &outVerts) const;
	// Update vertices according to the bones
	void UpdateVertices(StaticArray<Vertex5> &outVerts) const;
};
