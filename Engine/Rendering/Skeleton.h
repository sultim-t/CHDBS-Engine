#pragma once
#include "Bone.h"

class Skeleton
{
	friend class ResourceManager;

private:
	// Referenc to mesh which is affected by this skeleton
	const MeshResource		&mesh;

	// All bones in this skeleton
	// Order is not important, bone index is in Bone class
	StaticArray<Bone>		bones;

	// Bone weights for each vertex
	// Each element stores array of weights for each vertex
	StaticArray<VertexWeight>		vertexWeights;

	// Bone inverse bind transformations (from root to bone)
	StaticArray<Matrix4>			inverseBonesMatrices;

	// Temp bone transformations
	// Is used to calculate animation
	mutable StaticArray<Matrix4>	bonesMatrices;

private:
	Matrix4 GetBoneTranform(const Bone &bone) const;
	Matrix4 GetBoneTranform(const Bone &bone, const Animation *animation, float time) const;

	// Get animated local transformation of this node
	Matrix4 GetNodeTranform(const ModelNode &node, const Animation *animation, float time) const;
	
	void CalculateInverseBoneTransforms();

public:
	// Allocates memory for bone and vertex arrays
	Skeleton(const MeshResource &meshToUse, int bonesCount, int verticesCount);
	// Initialize this skeleton, must be called after setting all bones
	void Init();

	// Update bone transforms, should be called before UpdateVertices
	void UpdateBoneMatrices() const;

	// Update bone transforms according to animation
	void UpdateBoneMatrices(const Animation *animation, float time) const;

	// Update vertices according to the bones,
	// and load them to the buffer "outVerts"
	void UpdateVertices(StaticArray<Vertex5> &outVerts) const;
};
