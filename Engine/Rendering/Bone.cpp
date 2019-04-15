#include "Bone.h"
#include "Skeleton.h"

Bone::Bone(Skeleton *skeleton, int boneID, const ModelNode *modelNode, const Matrix4 & localOffsetMatrix)
	:skeleton(skeleton), boneID(boneID), parentBoneID(-1), modelNode(modelNode), localOffsetMatrix(localOffsetMatrix) 
{ }

void Bone::Init()
{	
	//this->skeleton = skeleton;
	//this->boneID = boneID;
	//this->modelNode = modelNode;
	//this->localOffsetMatrix = localOffsetMatrix;

	// init array
	childIDs.Init(4);
}

void Bone::operator=(const Bone & source)
{
	this->skeleton = source.skeleton;
	this->boneID = source.boneID;
	this->modelNode = source.modelNode;
	this->localOffsetMatrix = source.localOffsetMatrix;

	// default conruttucor
	this->childIDs = DynamicArray<int>();
}

const Matrix4 & Bone::GetInverseGlobalOffsetMatrix() const
{
	return inverseOffsetMatrix;
}

const ModelNode &Bone::GetModelNode() const
{
	return *modelNode;
}

int Bone::GetBoneID() const
{
	return boneID;
}

int Bone::GetParentBoneID() const
{
	return parentBoneID;
}

const DynamicArray<int> Bone::GetChildBonesID() const
{
	return childIDs;
}

void Bone::SetParentBone(int id)
{
	parentBoneID = id;
}

void Bone::AddChildBone(int id)
{
	childIDs.Push(id);
}

void Bone::CalculateInverseMatrix(const Matrix4 & parentBoneOffset)
{
	const Matrix4 &c = localOffsetMatrix * parentBoneOffset;

	// invert global (model) pose matrix
	inverseOffsetMatrix = c.GetInversed();

	// for each child bone
	for (int i = 0; i < childIDs.GetSize(); i++)
	{
		Bone *child = skeleton->GetBones()[childIDs[i]];
		child->CalculateInverseMatrix(c);
	}
}

const Matrix4 &Bone::GetOffsetMatrix() const
{
	return localOffsetMatrix;
}