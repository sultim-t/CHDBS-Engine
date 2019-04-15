#include "Skeleton.h"

#include "Animation.h"
#include "DebugDrawer.h"
#include <Engine/ResourceManager/MeshResource.h>
#include <Engine/Math/Transform.h>

Skeleton::Skeleton(const MeshResource &meshToUse, int bonesCount, int verticesCount)
	: mesh(meshToUse), rootBoneID(-1)
{
	// allocate for bones
	bones.Init(bonesCount);
	bonesMatrices.Init(bonesCount);

	// allocate for vertices
	vertexWeights.Init(verticesCount);

	// initialize weights with default values
	for (int i = 0; i < verticesCount; i++)
	{
		vertexWeights[i] = VertexWeight();
	}
}

Skeleton::~Skeleton()
{
	for (UINT i = 0; i < bones.GetSize(); i++)
	{
		delete bones[i];
	}
}

void Skeleton::Init()
{
	// find root bone
	FindRootBone();

	// init once all inverse matrices starting from root bone with identity transformation
	bones[rootBoneID]->CalculateInverseMatrix(Matrix4(1.0f, true));
}

void Skeleton::UpdateBoneMatrices() const
{
	for (UINT i = 0; i < bones.GetSize(); i++)
	{
		bonesMatrices[i] = Matrix4(1.0f, true);
	}

	// recursive call from root
	UpdateBoneMatrices(bones[rootBoneID], Matrix4(1.0f, true));
}

void Skeleton::UpdateBoneMatrices(const Animation *animation, float time) const
{	
	for (UINT i = 0; i < bones.GetSize(); i++)
	{
		bonesMatrices[i] = Matrix4(1.0f, true);
	}

	// recursive call from root
	UpdateBoneMatrices(bones[rootBoneID], animation, time, Matrix4(1.0f, true));
}

void Skeleton::UpdateBoneMatrices(const Bone *bone, const Matrix4 & parent) const
{
	// local node transform
	Matrix4 currentLocal = bone->GetModelNode().GetTransform();

	// node to global (model) space
	Matrix4 current = currentLocal * parent;

	// get array
	const DynamicArray<int> &childBones = bone->GetChildBonesID();
	int childCount = childBones.GetSize();

	// for each child bone
	for (int i = 0; i < childCount; i++)
	{
		// update recurcsively
		UpdateBoneMatrices(bones[childBones[i]], current);
	}

	// apply bone's default pose
	current = bone->GetInverseGlobalOffsetMatrix() * current;

	// set transposed to use vector multiplication
	bonesMatrices[bone->GetBoneID()] = current.GetTransposed();
}

void Skeleton::UpdateBoneMatrices(const Bone *bone, const Animation * animation, float time, const Matrix4 & parent) const
{
	// local node transform
	Matrix4 currentLocal = bone->GetModelNode().GetTransform(*animation, time);

	// node to global (model) space
	Matrix4 current = currentLocal * parent;

	// get array
	const DynamicArray<int> &childBones = bone->GetChildBonesID();
	int childCount = childBones.GetSize();

	// for each child bone
	for (int i = 0; i < childCount; i++)
	{
		// update recurcsively
		UpdateBoneMatrices(bones[childBones[i]], animation, time, current);
	}

	// apply bone's default pose
	current = bone->GetInverseGlobalOffsetMatrix() * current;

	// set transposed to use vector multiplication
	bonesMatrices[bone->GetBoneID()] = current.GetTransposed();
}

void Skeleton::FindRootBone()
{
	int b = 0;

	// while parent exist
	do
	{
		// update root bone
		rootBoneID = b;

		// get its parent
		b = bones[b]->GetParentBoneID();
	} while (b >= 0);
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

StaticArray<Bone*>& Skeleton::GetBones()
{
	return bones;
}