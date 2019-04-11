#include "Skeleton.h"
#include <Engine/ResourceManager/MeshResource.h>

Skeleton::Skeleton(const MeshResource &meshToUse, int bonesCount, int verticesCount)
	: mesh(meshToUse)
{
	// allocate for bones
	bones.Init(bonesCount);
	bonesMatrices.Init(bonesCount);

	// allocate for vertices
	vertexWeights.Init(verticesCount);

	// initialize weights
	for (int i = 0; i < verticesCount; i++)
	{
		vertexWeights[i] = VertexWeight();
	}
}

void Skeleton::UpdateBoneMatrices(const Matrix4 &global) const
{
	// for each bone
	for (UINT i = 0; i < bones.GetSize(); i++)
	{
		// tranform to original pose
		Matrix4 m = bones[i].GetOffsetMatrix();
		// to global node transform
		m *= GetBoneTranform(bones[i]);
		// to global
		// m *= global;

		bonesMatrices[i] = m.GetTransposed();
	}
}

Matrix4 Skeleton::GetBoneTranform(const Bone &bone) const
{
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

void Skeleton::UpdateVertices(const Matrix4 &global, StaticArray<Vertex5> &outVerts) const
{
	// update matrices for bones
	UpdateBoneMatrices(global);

	const StaticArray<Vertex5> &meshVerts = mesh.GetVertices();

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
			// use default position
			outVerts[i].Position = meshVerts[i].Position;

			continue;
		}

		// init with the first the first bone
		Matrix4 boneMatrix = bonesMatrices[weights[0].BoneIndex] * weights[0].Weight;

		// update position for each bone
		for (int j = 1; j < weightCount; j++)
		{
			// update matrix
			boneMatrix += bonesMatrices[weights[j].BoneIndex] * weights[j].Weight;
		}

		// update position in buffer
		outVerts[i].Position = boneMatrix * Vector4(meshVerts[i].Position, 1.0f);
	}
}

void Skeleton::UpdateVertices(StaticArray<Vertex5>& outVerts) const
{
	UpdateVertices(Matrix4(1.0f, true), outVerts);
}
