#pragma once
#include "Bone.h"

class Skeleton
{
	friend class ResourceManager;
	friend class Bone;

private:
	// Referenc to mesh which is affected by this skeleton
	const MeshResource		&mesh;

	// Root bone ID
	int						rootBoneID;

	// All bones in this skeleton
	// Order is not important, bone index is in Bone class
	StaticArray<Bone*>		bones;

	// Bone weights for each vertex
	// Each element stores array of weights for each vertex
	StaticArray<VertexWeight>		vertexWeights;

	// Temp bone transformations
	// Is used to calculate animation
	mutable StaticArray<Matrix4>	bonesMatrices;

private:
	// Get all bones in this skeleton
	// Todo: change to const (there is problem in CalculateInverseMatrix in Bone class)
	// temp solution : friend class Bone :(
	StaticArray<Bone*> &GetBones();

	// Recursive
	void UpdateBoneMatrices(const Bone *bone, const Matrix4 &parent) const;
	// Recursive
	void UpdateBoneMatrices(const Bone *bone, const Animation *animation, float time, const Matrix4 &parent) const;

	void FindRootBone();

public:
	// Allocates memory for bone and vertex arrays
	Skeleton(const MeshResource &meshToUse, int bonesCount, int verticesCount);
	~Skeleton();

	// Initialize this skeleton, must be called after setting all bones
	// Finds root bone
	void Init();

	// Update bone transforms, should be called before UpdateVertices
	void UpdateBoneMatrices() const;

	// Update bone transforms according to animation
	void UpdateBoneMatrices(const Animation *animation, float time) const;

	// Update vertices according to the bones,
	// and load them to the buffer "outVerts"
	void UpdateVertices(StaticArray<Vertex5> &outVerts) const;
};
