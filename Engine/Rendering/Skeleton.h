#pragma once

#include "Bone.h"

class Skeleton
{
private:
	// All bones in this skeleton
	// Order is not important
	StaticArray<Bone>		bones;
	StaticArray<Matrix4>	bonesMatrices;

public:
	inline Skeleton(int bonesCount);

	inline void UpdateBoneMatrices(const Matrix4 &global);
};

inline Skeleton::Skeleton(int bonesCount)
{
	bones.Init(bonesCount);
	bonesMatrices.Init(bonesCount);
}

inline void Skeleton::UpdateBoneMatrices(const Matrix4 &global)
{
	for (UINT i = 0; i < bones.GetSize(); i++)
	{
		Matrix4 m = bones[i].GetOffsetMatrix();
		m *= bones[i].GetModelNode().GetTransform();
		m *= bones[i].GetTranformationMatrix();
		m *= global;

		bonesMatrices[i] = m;
	}
}