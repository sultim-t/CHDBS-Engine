#include "Skeleton.h"

#include "Animation.h"
#include "DebugDrawer.h"
#include <Engine/ResourceManager/MeshResource.h>
#include <Engine/Math/Transform.h>

Skeleton::Skeleton(const MeshResource &meshToUse, int bonesCount, int verticesCount)
	: mesh(meshToUse)
{
	// allocate for bones
	bones.Init(bonesCount);
	bonesMatrices.Init(bonesCount);
	inverseBonesMatrices.Init(bonesCount);

	// allocate for vertices
	vertexWeights.Init(verticesCount);

	// initialize weights with default values
	for (int i = 0; i < verticesCount; i++)
	{
		vertexWeights[i] = VertexWeight();
	}
}

void Skeleton::Init()
{
	CalculateInverseBoneTransforms();
}

void Skeleton::CalculateInverseBoneTransforms()
{
	int bonesCount = bones.GetSize();

	for (int i = 0; i < bonesCount; i++)
	{

		// global bone default pose transformation
		Matrix4 globalBoneTransform = bones[i].GetOffsetMatrix();

		int b = bones[i].GetParentBoneID();

		// while parent bone exist
		while (b > 0)
		{
			// get parent bone tranformation
			const Matrix4 &parentTransform = bones[b].GetOffsetMatrix();

			// convert to parent's space
			globalBoneTransform *= parentTransform;

			// get parent's parent ID
			b = bones[b].GetParentBoneID();
		}

		inverseBonesMatrices[i] = globalBoneTransform.GetInversed();
	}
}

void Skeleton::UpdateBoneMatrices() const
{
	// for each bone
	for (UINT i = 0; i < bones.GetSize(); i++)
	{
		// tranform to original pose
		Matrix4 m = bones[i].GetOffsetMatrix();
		// to global node transform
		m *= GetBoneTranform(bones[i]);

		bonesMatrices[i] = m.GetTransposed();
	}
}

Matrix4 Skeleton::GetBoneTranform(const Bone &bone) const
{
	// NOTE: it's very unoptimized way to update transformations
	// because calculated matrices are not saved

	Matrix4 result = bone.GetModelNode().GetTransform();
	int b = bone.GetParentBoneID();

	// while parent bone exist
	while (b > 0)
	{
		// get parent bone tranformation
		const Matrix4 &parentTransform = bones[b].GetModelNode().GetTransform();

		// convert to parent's space
		result *= parentTransform;

		// get parent's parent ID
		b = bones[b].GetParentBoneID();
	}

	return result;
}

void Skeleton::UpdateBoneMatrices(const Animation *animation, float time) const
{
	// for each bone
	for (UINT i = 0; i < bones.GetSize(); i++)
	{
		// tranform to original pose
		Matrix4 m = bones[i].GetOffsetMatrix();
		
		//Matrix4 m = inverseBonesMatrices[i];
		
		// to global node transform
		m *= GetBoneTranform(bones[i], animation, time);

		bonesMatrices[i] = m.GetTransposed();
	}
}

Matrix4 Skeleton::GetBoneTranform(const Bone &bone, const Animation *animation, float time) const
{	
	// NOTE: it's very unoptimized way to update transformations
	// because calculated matrices are not saved

	// get animated local node tranformation
	Matrix4 result = GetNodeTranform(bone.GetModelNode(), animation, time);
	int b = bone.GetParentBoneID();

	// while parent bone exist
	while (b > 0)
	{
		// get parent local bone tranformation
		const Matrix4 &parentTransform = GetNodeTranform(bones[b].GetModelNode(), animation, time);

		// convert to parent's space
		result *= parentTransform;

		// get parent's parent ID
		b = bones[b].GetParentBoneID();
	}

	// ok, transformation matrix in model space
	return result;
}

Matrix4 Skeleton::GetNodeTranform(const ModelNode & node, const Animation * animation, float time) const
{
	// base local tranformation
	Matrix4 result = node.GetTransform();

	// try to find animation node with the same name
	const AnimationNode *animNode = animation->FindAnimationNode(node.GetName());

	// if found
	if (animNode != nullptr)
	{
		Vector3 position;
		Quaternion quat;
		Vector3 scale;

		// 1) animate position
		if (animNode->GetInterpolatedPosition(time, position))
		{
			result = Transform::TranslateMatrix(result, position);
		}

		// 2) animate scale
		/*if (animNode->GetInterpolatedScale(time, scale))
		{
			result = Transform::TranslateMatrix(result, scale);
		}*/

		// 3) animate rotation
		if (animNode->GetInterpolatedRotation(time, quat))
		{
			result = Transform::RotateMatrix(result, quat);
		}
	}

	return result;
}

void Skeleton::UpdateVertices(StaticArray<Vertex5> &outVerts) const
{
	const StaticArray<Vertex5> &meshVerts = mesh.GetVertices();

	// buffer must be equal or bigger
	ASSERT(meshVerts.GetSize() <= outVerts.GetSize());

	// now update all vertices
	UINT size = vertexWeights.GetSize();
	// for each vertex
	for (UINT i = 0; i < size; i++)
	{
		// get current weight
		const VertexWeight &vertexWeight = vertexWeights[i];

		int weightCount = vertexWeight.WeightCount;
		const BoneWeight *weights = vertexWeight.Weights;

		// no weights
		if (weightCount == 0)
		{
			// use default
			outVerts[i].Position = meshVerts[i].Position;
			outVerts[i].Normal = meshVerts[i].Normal;

			continue;
		}

		// init with the first matrix
		Matrix4 boneMatrix = bonesMatrices[weights[0].BoneIndex] * weights[0].Weight;

		// update for each weight
		for (int j = 1; j < weightCount; j++)
		{
			boneMatrix += bonesMatrices[weights[j].BoneIndex] * weights[j].Weight;
		}

		// update position and normal in buffer
		// boneMatrix is transposed => can multiply vertices
		outVerts[i].Position = boneMatrix * Vector4(meshVerts[i].Position, 1.0f);
		outVerts[i].Normal = boneMatrix * Vector4(meshVerts[i].Normal, 0.0f);
	}
}